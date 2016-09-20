#include<cassert>

#include"slice_iterator.hpp"

clipboard::clipboard(void):
stored(create_quotation(0)), // dummy token
up_to_date(false),
frozen(true){
}

void clipboard::paste(const token& t)throw(std::string){
    if(up_to_date){
        stored+=(t);
        frozen=false;
    }
    else{
        stored=t;
        frozen=false;
        up_to_date=true;
    }
}

void clipboard::report_end(void){
    frozen = true;
}

void clipboard::report_next_token(void){
    up_to_date = false;
    frozen = true;
}

void clipboard::paste_null_token(void){
    up_to_date = false;
    frozen = false;
}

bool clipboard::contains_full_token(void)const{
    return up_to_date && frozen;
}

bool clipboard::contains_some_token(void)const{
    return up_to_date;
}

bool clipboard::should_paste_into(void)const{
    return !frozen;
}

token clipboard::get_current_token()const{
    assert(contains_full_token());
    return stored;
}

backtrace_info::backtrace_info(const slice& s):
current_slice(s),
current_begin(0),
current_end(0),
started(false),
last_should_be_pasted(false){
}

backtrace_info::backtrace_info(const backtrace_info& src):
current_slice(src.current_slice),
current_begin(src.current_begin),
current_end(src.current_end),
started(src.started),
last_should_be_pasted(src.last_should_be_pasted){
}

backtrace_info::backtrace_info(backtrace_info&& src):
current_slice(std::move(src.current_slice)),
current_begin(src.current_begin),
current_end(src.current_end),
started(src.started),
last_should_be_pasted(src.last_should_be_pasted){
}

backtrace_info::~backtrace_info(void){
}

std::vector<slice> backtrace_info::parse_arguments(messages_container& msg)throw(message){
    std::vector<slice> result;
    ++current_end;
    if(current_slice.is_end(current_end))
        throw msg.build_message(current_slice.get_token(current_end-1).get_position(),"Unexpected end of macro arguments");
    if(current_slice.get_token(current_end).get_type() != left_round_bracket)
        throw msg.build_message(current_slice.get_token(current_end).get_position(),"Expected \'(\' token, encountered \'"+current_slice.get_token(current_end).to_string()+"\'");
    while(true){
        ++current_end;
        if(current_slice.is_end(current_end))
            throw msg.build_message(current_slice.get_token(current_end-1).get_position(),"Unexpected end of macro arguments");
        uint bracket_balance = 0;
        uint slice_begin = current_end;
        while(!current_slice.is_end(current_end)
            && ((current_slice.get_token(current_end).get_type() != comma && current_slice.get_token(current_end).get_type() != right_round_bracket)
                || bracket_balance > 0)){
            if(current_slice.get_token(current_end).get_type() == left_round_bracket)
                ++bracket_balance;
            if(current_slice.get_token(current_end).get_type() == right_round_bracket)
                --bracket_balance;
            ++current_end;
        }
        uint slice_end = current_end;
        result.push_back(current_slice.cut_slice(slice_begin,slice_end));
        if(current_slice.is_end(current_end))
            throw msg.build_message(current_slice.get_token(current_end-1).get_position(),"Unexpected end of macro arguments");
        if(current_slice.get_token(current_end).get_type() == right_round_bracket)
            return result;
    }
}

void backtrace_info::rewind_parsing_arguments(void){
    current_end = current_begin;
}

void backtrace_info::set_last_should_be_pasted(void){
    last_should_be_pasted = true;
}

bool backtrace_info::get_last_should_be_pasted(void){
    return last_should_be_pasted;
}

uint backtrace_info::get_context_order(void)const{
    return current_slice.get_context_order();
}

bool backtrace_info::points_at_variable(void)const{
    return has_value() && current_slice.should_be_replaced(current());
}

bool backtrace_info::next_is_tilde(void)const{
    return !current_slice.is_end(current_end+1) && current_slice.get_token(current_end+1).get_type() == tilde;
}

bool backtrace_info::is_last_token_in_slice(void)const{
    return has_value() && current_slice.is_end(current_end+1);
}

bool backtrace_info::is_first_token_in_slice(void)const{
    return current_end == 0;
}

slice backtrace_info::get_variable_slice(void)const{
    return current_slice.get_replacement(current());
}

token backtrace_info::get_beginning(void)const{
    assert(!current_slice.is_end(current_begin));
    return current_slice.get_token(current_begin);
}

token backtrace_info::current(void)const{
    assert(current_begin == current_end);
    return current_slice.get_token(current_end);
}

void backtrace_info::next(void){
    if(started)
        ++current_end;
    started = true;
    current_begin = current_end;
}

bool backtrace_info::has_value(void)const{
    return !current_slice.is_end(current_end);
}

slice_iterator::slice_iterator(const slice& s):
backtrace_stack(),
c(){
    backtrace_stack.push_back(s);
}

std::vector<std::pair<uint,std::string>> slice_iterator::create_call_stack(const std::string& details)const{
    assert(!backtrace_stack.empty());
    std::vector<std::pair<uint,std::string>> result;
    for(uint i=0;i<backtrace_stack.size()-1;++i){
        token t = backtrace_stack[i].get_beginning();
        if(backtrace_stack[i].get_context_order() > backtrace_stack[i+1].get_context_order())
            result.push_back(std::make_pair(t.get_position(),"in expansion of macro \'"+t.to_string()+"\'"));
        else
            result.push_back(std::make_pair(t.get_position(),"in expansion of \'"+t.to_string()+"\' macro argument"));
    }
    result.push_back(std::make_pair(backtrace_stack.back().current().get_position(),details));
    return result;
}

bool slice_iterator::has_value(void)const{
    return !backtrace_stack.empty() || c.contains_full_token();
}

token slice_iterator::current(void)const{
    if(c.contains_full_token())
        return c.get_current_token();
    else
        return backtrace_stack.back().current();
}

void slice_iterator::push_next_slice(slice&& s){
    bool last_to_paste = backtrace_stack.back().next_is_tilde() || (backtrace_stack.back().is_last_token_in_slice() && backtrace_stack.back().get_last_should_be_pasted());
    backtrace_stack.push_back(std::move(s));
    if(last_to_paste)
        backtrace_stack.back().set_last_should_be_pasted();
}

void slice_iterator::pop_slice(void){
    if(backtrace_stack.back().get_last_should_be_pasted() && !c.contains_some_token())
        c.paste_null_token();
    if(!backtrace_stack.back().get_last_should_be_pasted())
        c.report_end();
    backtrace_stack.pop_back();
    if(!backtrace_stack.empty() && backtrace_stack.back().next_is_tilde())
        backtrace_stack.back().next();
}

bool slice_iterator::handle_standard_token(messages_container& msg)throw(message){
    try{
        if(backtrace_stack.back().is_last_token_in_slice() && backtrace_stack.back().get_last_should_be_pasted()){
            c.paste(backtrace_stack.back().current());
            return false;
        }
        if(backtrace_stack.back().next_is_tilde()){
            c.paste(backtrace_stack.back().current());
            backtrace_stack.back().next();
            if(backtrace_stack.back().get_context_order() == backtrace_stack[0].get_context_order())
                throw msg.build_message(backtrace_stack.back().current().get_position(),"Tokens can be pasted only inside macros");
            if(backtrace_stack.back().is_last_token_in_slice())
                throw msg.build_message(backtrace_stack.back().current().get_position(),"Tilde at the end of tokens string");
            return false;
        }
        else{
            if(c.should_paste_into())
                c.paste(backtrace_stack.back().current());
            c.report_end();
            return true;
        }
    }
    catch(const std::string& m){
        throw msg.build_message(create_call_stack(m));
    }
}

bool slice_iterator::next(const macro_bank& macros,messages_container& msg)throw(message){
    c.report_next_token();
    while(true){
        while(!backtrace_stack.empty()){
            if(c.contains_full_token())
                return true;
            backtrace_stack.back().next();
            if(!backtrace_stack.back().has_value())
                pop_slice();
            else
                break;
        }
        if(backtrace_stack.empty())
            return false;
        if(backtrace_stack.back().current().get_type() == tilde)
            throw msg.build_message(backtrace_stack.back().current().get_position(),"Unexpected tilde");
        else if(backtrace_stack.back().points_at_variable())
            push_next_slice(backtrace_stack.back().get_variable_slice());
        else if(macros.could_be_macro_name(backtrace_stack.back().current())){
            if(macros.is_zero_arity_macro(backtrace_stack.back().current(),backtrace_stack.back().get_context_order()))
                push_next_slice(macros.get_macro_slice(backtrace_stack.back().current(),std::vector<slice>()));
            else if(macros.is_non_zero_arity_macro(backtrace_stack.back().current(),backtrace_stack.back().get_context_order())){
                token name = backtrace_stack.back().current();
                std::vector<slice> args = backtrace_stack.back().parse_arguments(msg);
                if(!macros.is_macro_name_and_arity(name,args.size(),backtrace_stack.back().get_context_order()))
                    throw msg.build_message(name.get_position(),"Macro with given name exists but requires another number of arguments ("+std::to_string(args.size())+" given)");
                push_next_slice(macros.get_macro_slice(name,args));
            }
            else if(handle_standard_token(msg))
                return true;
        }
        else if(handle_standard_token(msg))
            return true;
    }
}

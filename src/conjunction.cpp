#include"conjunction.hpp"
#include"negatable_condition.hpp"
#include"printer_helpers.hpp"

namespace rbg_parser{

conjunction::conjunction(std::vector<std::unique_ptr<condition>> content):
content(std::move(content)){
}

void conjunction::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

const std::vector<std::unique_ptr<condition>>& conjunction::get_content(void)const{
    return content;
}

std::unique_ptr<condition> conjunction::simplify(void){
    std::vector<std::unique_ptr<condition>> result;
    for(uint i=0;i<content.size();++i)
        result.push_back(content[i]->simplify());
    if(result.size()==1)
        return std::move(result[0]);
    else
        return std::unique_ptr<condition>(new conjunction(std::move(result)));
}

std::string conjunction::to_rbg(uint indent)const{
    std::string result = "";
    for(uint i=0;i<content.size();++i){
        result += (i==0 ? "" : " and ");
        result += open_bracket_if_necessary(priority(),content[i]->priority());
        result += content[i]->to_rbg(indent);
        result += close_bracket_if_necessary(priority(),content[i]->priority());
    }
    return result;
}

std::string conjunction::to_rbg()const{
    std::string result = "";
    for(uint i=0;i<content.size();++i){
        result += (i==0 ? "" : " and ");
        result += open_bracket_if_necessary(priority(),content[i]->priority());
        result += content[i]->to_rbg();
        result += close_bracket_if_necessary(priority(),content[i]->priority());
    }
    return result;
}

parser_result<conjunction> parse_conjunction(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing condition conjunction");
    std::vector<std::unique_ptr<condition>> result;
    bool already_started = false;
    while(true){
        if(already_started){
            if(!it.has_value() || it.current(msg).get_type() != logical_and)
                return success(conjunction(std::move(result)));
            else
                it.next(msg);
        }
        auto lower_result = parse_negatable_condition(it,decls,msg);
        if(!lower_result.is_success()){
            if(already_started)
                throw msg.build_message(it.create_call_stack("Expected elementary condition, \'not\' or \'(\', encountered \'"+it.current(msg).to_string()+"\'"));
            else
                return failure<conjunction>();
        }
        auto next_elem = std::unique_ptr<condition>(new negatable_condition(lower_result.move_value()));
        result.push_back(std::move(next_elem));
        already_started = true;
    }
}

}

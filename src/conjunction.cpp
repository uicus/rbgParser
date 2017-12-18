#include"conjunction.hpp"
#include"negatable_condition.hpp"

conjunction::conjunction(std::vector<std::unique_ptr<condition>> content):
content(std::move(content)){
}

void conjunction::accept(abstract_dispatcher& dispatcher){
    dispatcher.dispatch(*this);
}

const std::vector<std::unique_ptr<condition>>& conjunction::get_content(void)const{
    return content;
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

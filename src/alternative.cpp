#include"alternative.hpp"
#include"conjunction.hpp"

alternative::alternative(std::vector<std::unique_ptr<condition>> content):
content(std::move(content)){
}

parser_result<alternative> parse_alternative(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing condition alternative");
    std::vector<std::unique_ptr<condition>> result;
    bool already_started = false;
    while(true){
        if(already_started){
            if(!it.has_value() || it.current(msg).get_type() != logical_or)
                return success(alternative(std::move(result)));
            else
                it.next(msg);
        }
        auto lower_result = parse_conjunction(it,decls,msg);
        if(!lower_result.is_success()){
            if(already_started)
                throw msg.build_message(it.create_call_stack("Expected condition conjunction, encountered \'"+it.current(msg).to_string()+"\'"));
            else
                return failure<alternative>();
        }
        auto next_elem = std::unique_ptr<condition>(new conjunction(lower_result.move_value()));
        result.push_back(std::move(next_elem));
        already_started = true;
    }
}

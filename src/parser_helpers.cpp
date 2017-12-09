#include"parser_helpers.hpp"
#include"types.hpp"

parser_result<int> parse_int(slice_iterator& it, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing integer");
    bool is_positive = true;
    if(!it.has_value())
        return failure<int>();
    if(it.current(msg).get_type() == plus)
        it.next(msg);
    else if(it.current(msg).get_type() == minus){
        is_positive = false;
        it.next(msg);
    }
    else if(it.current(msg).get_type() != number)
        return failure<int>();
    if(it.current(msg).get_type() != number)
        throw msg.build_message(it.create_call_stack("Expected digit, encountered \'"+it.current(msg).to_string()+"\'"));
    int result = is_positive ? int(it.current(msg).get_value()) : -int(it.current(msg).get_value());
    it.next(msg);
    return success(std::move(result));
}

parser_result<std::vector<token>> parse_sequence(
slice_iterator& it,
const std::string& purpose_name,
const std::set<token>& verification_set,
bool should_verify,
messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing "+purpose_name);
    std::vector<token> result;
    if(!it.has_value())
        return success(std::move(result));
    bool should_meet_comma = false;
    while(it.has_value() && (it.current(msg).get_type() == comma || it.current(msg).get_type() == identifier)){
        if(it.current(msg).get_type() == comma)
            should_meet_comma = false;
        else if(it.current(msg).get_type() == identifier){
            if(should_meet_comma)
                throw msg.build_message(it.create_call_stack("Two identifiers should be separated by at least one comma"));
            else if(!should_verify || verification_set.find(it.current(msg)) != verification_set.end())
                result.push_back(it.current(msg));
            else
                throw msg.build_message(it.create_call_stack("Identifier \'"+it.current(msg).to_string()+"\' was not declared in respective segment"));
            should_meet_comma = true;
        }
        it.next(msg);
    }
    return success(std::move(result));
}

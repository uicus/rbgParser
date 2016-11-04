#include"parser_helpers.hpp"
#include"types.hpp"

parser_result<int> parse_int(slice_iterator& it, messages_container& msg)throw(message){
    bool is_positive = true;
    if(!it.has_value())
        return failure<int>();
    if(it.current().get_type() == plus)
        it.next(msg);
    else if(it.current().get_type() == minus){
        is_positive = false;
        it.next(msg);
    }
    else if(it.current().get_type() != number)
        return failure<int>();
    if(!it.has_value())
        throw msg.build_message("Unexpected end of number");
    if(it.current().get_type() != number)
        throw msg.build_message(it.create_call_stack("Expected digit, encountered \'"+it.current().to_string()+"\'"));
    int result = is_positive ? int(it.current().get_value()) : -int(it.current().get_value());
    it.next(msg);
    return success<int>(std::move(result));
}

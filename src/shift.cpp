#include"shift.hpp"

shift::shift(int x,int y):
x(x),
y(y){
}

shift::shift(void):
x(0),
y(0){
}

parser_result<shift> parse_shift(slice_iterator& it, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing \'shift\' move");
    auto x_result = parse_int(it,msg);
    if(!x_result.is_success())
        return failure<shift>();
    int x = x_result.get_value();
    if(it.current(msg).get_type() != comma)
        throw msg.build_message(it.create_call_stack("Expected \',\', encountered \'"+it.current(msg).to_string()+"\'"));
    if(!it.next(msg))
        throw msg.build_message("Unexpected end of input after \',\' while parsing shift move");
    auto y_result = parse_int(it,msg);
    if(!y_result.is_success())
        throw msg.build_message(it.create_call_stack("Expected integer, encountered \'"+it.current(msg).to_string()+"\'"));
    int y = y_result.get_value();
    return success(shift(x,y));
}

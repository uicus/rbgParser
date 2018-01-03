#include"shift.hpp"

namespace rbg_parser{

shift::shift(int x,int y):
x(x),
y(y){
}

parser_result<shift> parse_shift(slice_iterator& it, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing \'shift\' move");
    auto x_result = parse_int(it,msg);
    if(!x_result.is_success())
        return failure<shift>();
    int x = x_result.get_value();
    if(it.current(msg).get_type() != comma)
        throw msg.build_message(it.create_call_stack("Expected \',\', encountered \'"+it.current(msg).to_string()+"\'"));
    it.next(msg);
    auto y_result = parse_int(it,msg);
    if(!y_result.is_success())
        throw msg.build_message(it.create_call_stack("Expected integer, encountered \'"+it.current(msg).to_string()+"\'"));
    int y = y_result.get_value();
    return success(shift(x,y));
}

void shift::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string shift::to_rbg(uint)const{
    return to_rbg();
}

std::unique_ptr<pure_game_move> shift::pure_flatten(void){
    return std::unique_ptr<pure_game_move>(new shift(std::move(*this)));
}

std::string shift::to_rbg()const{
    std::string result = "(";
    result += std::to_string(x);
    result += ",";
    result += std::to_string(y);
    result += ")";
    return result;
}

int shift::get_x(void)const{
    return x;
}

int shift::get_y(void)const{
    return y;
}

}

#include"period_leaf.hpp"
#include"slice_iterator.hpp"
#include"message.hpp"
#include"noop.hpp"

namespace rbg_parser{

period_leaf::period_leaf(slice_iterator&& beginning_position):
expression(std::move(beginning_position)){
}

expression_type period_leaf::get_type(void)const{
    return gmove;
}

void period_leaf::type(const typing_machine&, messages_container&){
}

std::unique_ptr<game_move> period_leaf::get_game_move(void)const{
    return std::unique_ptr<game_move>(new noop);
}

parser_result<std::unique_ptr<expression>> parse_period_leaf(slice_iterator& it, messages_container& msg){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing period");
    auto beginning = it;
    if(not it.has_value())
        return failure<std::unique_ptr<expression>>();
    else if(it.current(msg).get_type() != period)
        return failure<std::unique_ptr<expression>>();
    std::unique_ptr<expression> result(new period_leaf(std::move(beginning)));
    it.next(msg);
    return success(std::move(result));
}

}

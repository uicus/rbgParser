#include"arrow_leaf.hpp"
#include"slice_iterator.hpp"
#include"message.hpp"
#include"typing_machine.hpp"
#include"switch.hpp"

namespace rbg_parser{

arrow_leaf::arrow_leaf(slice_iterator&& beginning_position, const token& next, bool has_next):
expression(std::move(beginning_position)),
next(next),
has_next(has_next){
}

expression_type arrow_leaf::get_type(void)const{
    return gmove;
}

void arrow_leaf::type(const typing_machine&, messages_container&){
}

std::unique_ptr<game_move> arrow_leaf::get_game_move(void)const{
    if(has_next){
        if(next.get_type() == star)
            return std::unique_ptr<game_move>(new keeper_switch(false));
        else
            return std::unique_ptr<game_move>(new player_switch(next));
    }
    else
        return std::unique_ptr<game_move>(new keeper_switch(true));
}

std::unique_ptr<condition> arrow_leaf::get_condition(void)const{
    return get_game_move();
}

parser_result<std::unique_ptr<expression>> parse_arrow_leaf(slice_iterator& it, messages_container& msg){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing arrow expression");
    auto beginning = it;
    if(not it.has_value())
        return failure<std::unique_ptr<expression>>();
    if(it.current(msg).get_type() != arrow)
        return failure<std::unique_ptr<expression>>();
    it.next(msg);
    if(not it.has_value())
        return success(std::unique_ptr<expression>(new arrow_leaf(std::move(beginning), token(), false)));
    if(it.current(msg).get_type() == star or it.current(msg).get_type() == identifier){
        std::unique_ptr<expression> result(new arrow_leaf(std::move(beginning), it.current(msg)));
        it.next(msg);
        return success(std::move(result));
    }
    else
        return success(std::unique_ptr<expression>(new arrow_leaf(std::move(beginning), token(), false)));
}

}

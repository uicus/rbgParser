#include"sloth_leaf.hpp"
#include"slice_iterator.hpp"
#include"message.hpp"
#include"typing_machine.hpp"
#include"sloth.hpp"

namespace rbg_parser{

sloth_leaf::sloth_leaf(slice_iterator&& beginning_position):
expression(std::move(beginning_position)){
}

expression_type sloth_leaf::get_type(void)const{
    return gmove;
}

void sloth_leaf::type(const typing_machine&, messages_container&)throw(message){
}

std::unique_ptr<game_move> sloth_leaf::get_game_move(void)const{
    return std::unique_ptr<game_move>(new sloth);
}

parser_result<std::unique_ptr<expression>> parse_sloth_leaf(slice_iterator& it, messages_container& msg)throw(message){
    if(not it.has_value() || it.current(msg).get_type() != at_sign)
        return failure<std::unique_ptr<expression>>();
    auto beginning = it;
    it.next(msg);
    return success(std::unique_ptr<expression>(new sloth_leaf(std::move(beginning))));
}

}

#include"identifier_leaf.hpp"
#include"slice_iterator.hpp"
#include"message.hpp"
#include"typing_machine.hpp"
#include"shift.hpp"
#include"variable_arithmetic.hpp"

namespace rbg_parser{

identifier_leaf::identifier_leaf(slice_iterator&& beginning_position, const token& name):
expression(std::move(beginning_position)),
name(name),
identifier_type(not_typed_yet){
}

expression_type identifier_leaf::get_type(void)const{
    return identifier_type;
}

void identifier_leaf::type(const typing_machine& m, messages_container& msg){
    auto result = m.evaluate_identifier(name);
    if(result == error_type)
        throw msg.build_message(beginning_position.create_call_stack("Identifier \'"+name.to_string()+"\' not declared"));
    identifier_type = result;
}

token identifier_leaf::get_identifier(void)const{
    assert(identifier_type != current_player);
    return name;
}

std::vector<token> identifier_leaf::get_identifiers_sequence(void)const{
    assert(identifier_type != current_player);
    std::vector<token> result{name};
    return std::move(result);
}

std::unique_ptr<game_move> identifier_leaf::get_game_move(void)const{
    assert(identifier_type == shift_move);
    return std::unique_ptr<game_move>(new shift(name));
}

std::unique_ptr<arithmetic_expression> identifier_leaf::get_arithmetic_expression(void)const{
    assert(identifier_type != current_player);
    return std::unique_ptr<arithmetic_expression>(new variable_arithmetic(name));
}

std::unique_ptr<condition> identifier_leaf::get_condition(void)const{
    assert(identifier_type == shift_move);
    return get_game_move();
}

parser_result<std::unique_ptr<expression>> parse_identifier_leaf(slice_iterator& it, messages_container& msg){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing identifier");
    auto beginning = it;
    if(not it.has_value())
        return failure<std::unique_ptr<expression>>();
    else if(it.current(msg).get_type() != identifier && it.current(msg).get_type() != player && it.current(msg).get_type() != star)
        return failure<std::unique_ptr<expression>>();
    std::unique_ptr<expression> result(new identifier_leaf(std::move(beginning), it.current(msg)));
    it.next(msg);
    return success(std::move(result));
}

}

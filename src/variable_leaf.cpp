#include"variable_leaf.hpp"
#include"slice_iterator.hpp"
#include"message.hpp"
#include"typing_machine.hpp"
#include"variable_arithmetic.hpp"

namespace rbg_parser{

variable_leaf::variable_leaf(slice_iterator&& beginning_position, const token& name):
expression(std::move(beginning_position)),
name(name),
identifier_type(not_typed_yet){
}

expression_type variable_leaf::get_type(void)const{
    return identifier_type;
}

void variable_leaf::type(const typing_machine& m, messages_container& msg)throw(message){
    auto result = m.evaluate_variable_identifier(name);
    if(result == error_type)
        throw msg.build_message(beginning_position.create_call_stack("Variable \'"+name.to_string()+"\' not declared"));
    identifier_type = result;
}

token variable_leaf::get_identifier(void)const{
    return name;
}

std::unique_ptr<arithmetic_expression> variable_leaf::get_arithmetic_expression(void)const{
    return std::unique_ptr<arithmetic_expression>(new variable_arithmetic(name));
}

parser_result<std::unique_ptr<expression>> parse_variable_leaf(slice_iterator& it, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing variable");
    auto beginning = it;
    if(not it.has_value())
        return failure<std::unique_ptr<expression>>();
    else if(it.current(msg).get_type() != dollar)
        return failure<std::unique_ptr<expression>>();
    it.next(msg);
    if(it.current(msg).get_type() != identifier && it.current(msg).get_type() != player)
        throw msg.build_message(it.create_call_stack("Expected identifier or \'player\' after \'$\', encountered \'"+it.current(msg).to_string()+"\'"));
    std::unique_ptr<expression> result(new variable_leaf(std::move(beginning), it.current(msg)));
    it.next(msg);
    return success(std::move(result));
}

}

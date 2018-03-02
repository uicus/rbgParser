#include"identifier_leaf.hpp"
#include"slice_iterator.hpp"
#include"message.hpp"
#include"typing_machine.hpp"

namespace rbg_parser{

identifier_leaf::identifier_leaf(slice_iterator&& beginning_position, const token& name):
expression(std::move(beginning_position)),
name(name),
identifier_type(not_typed_yet){
}

expression_type identifier_leaf::get_type(void)const{
    return identifier_type;
}

void identifier_leaf::type(const typing_machine& m, messages_container& msg)throw(message){
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

parser_result<std::unique_ptr<expression>> parse_identifier_leaf(slice_iterator& it, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing identifier");
    auto beginning = it;
    if(not it.has_value())
        return failure<std::unique_ptr<expression>>();
    else if(it.current(msg).get_type() != identifier && it.current(msg).get_type() != player)
        return failure<std::unique_ptr<expression>>();
    std::unique_ptr<expression> result(new identifier_leaf(std::move(beginning), it.current(msg)));
    it.next(msg);
    return success(std::move(result));
}

}

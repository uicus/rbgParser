#include"move_condition.hpp"
#include"sum.hpp"

move_condition::move_condition(std::unique_ptr<pure_game_move> content):
content(std::move(content)){
}

void move_condition::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

const pure_game_move* move_condition::get_content(void)const{
    return content.get();
}

std::unique_ptr<condition> move_condition::simplify(void){
    return std::unique_ptr<condition>(new move_condition(content->pure_simplify()));
}

parser_result<move_condition> parse_move_condition(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing move pattern inside condition");
    if(!it.has_value() || it.current(msg).get_type() != exclamation)
        return failure<move_condition>();
    it.next(msg);
    auto move_begin = it;
    auto result = parse_sum(it,decls,msg);
    if(!result.is_success())
        throw msg.build_message(it.create_call_stack("Expected move pattern, encountered \'"+it.current(msg).to_string()+"\'"));
    auto sum_result = result.move_value();
    if(sum_result.modifies())
        throw msg.build_message(move_begin.create_call_stack("Moves after \'!\' cannot contain modifiers (i.e. moves inside \'[]\')"));
    auto pure_sum_result = sum_result.transform_into_pure();
    return success(move_condition(std::move(pure_sum_result)));
}

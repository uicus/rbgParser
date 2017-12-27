#include"condition_check.hpp"
#include"alternative.hpp"

condition_check::condition_check(std::unique_ptr<condition> content):
content(std::move(content)){
}

void condition_check::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

const condition* condition_check::get_content(void)const{
    return content.get();
}

std::unique_ptr<pure_game_move> condition_check::pure_simplify(void){
    return std::unique_ptr<pure_game_move>(new condition_check(content->simplify()));
}

parser_result<condition_check> parse_condition_check(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing condition");
    if(!it.has_value() || it.current(msg).get_type() != question)
        return failure<condition_check>();
    it.next(msg);
    auto condition_result = parse_alternative(it,decls,msg);
    if(!condition_result.is_success())
        throw msg.build_message(it.create_call_stack("Expected condition, encountered \'"+it.current(msg).to_string()+"\'"));
    return success(condition_check(std::unique_ptr<condition>(new alternative(condition_result.move_value()))));
}

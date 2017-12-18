#include"ons.hpp"

ons::ons(std::vector<token>&& legal_ons):
legal_ons(legal_ons.begin(),legal_ons.end()){
}

void ons::accept(abstract_dispatcher& dispatcher){
    dispatcher.dispatch(*this);
}

const std::set<token>& ons::get_legal_ons(void)const{
    return legal_ons;
}

parser_result<ons> parse_ons(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing \'ons\' move");
    auto set_result = parse_sequence(it,"legal ons",decls.get_legal_pieces(),true,msg);
    if(!set_result.is_success())
        return failure<ons>();
    else
        return success(ons(set_result.move_value()));
}

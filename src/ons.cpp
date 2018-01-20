#include"ons.hpp"
#include"printer_helpers.hpp"

namespace rbg_parser{

ons::ons(std::vector<token>&& legal_ons):
legal_ons(legal_ons.begin(),legal_ons.end()){
}

void ons::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string ons::to_rbg(uint)const{
    return to_rbg();
}

std::string ons::to_rbg()const{
    std::string result = "(";
    result += rbg_parser::to_rbg(legal_ons);
    result += ")";
    return result;
}

std::unique_ptr<pure_game_move> ons::pure_flatten(void){
    return std::unique_ptr<pure_game_move>(new ons(std::move(*this)));
}

const std::set<token>& ons::get_legal_ons(void)const{
    return legal_ons;
}

bool ons::finalizer_elligible(void)const{
    return true;
}

parser_result<ons> parse_ons(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing \'ons\' move");
    auto set_result = parse_sequence(it,"legal ons",decls.get_legal_pieces(),true,msg);
    if(!set_result.is_success())
        return failure<ons>();
    else
        return success(ons(set_result.move_value()));
}

}

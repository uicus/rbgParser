#include"player_check.hpp"
#include"printer_helpers.hpp"

namespace rbg_parser{

player_check::player_check(token player_to_check, bool equal_check):
player_to_check(player_to_check),
equal_check(equal_check){
}

void player_check::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string player_check::to_rbg(uint)const{
    return to_rbg();
}

std::string player_check::to_rbg()const{
    std::string result = "";
    result += "player";
    result += equal_check ? "==" : "!=";
    result += player_to_check.to_string();
    return result;
}

std::unique_ptr<condition> player_check::condition_simplify(void){
    return std::unique_ptr<condition>(new player_check(std::move(*this)));
}

std::unique_ptr<condition> player_check::condition_flatten(void){
    return std::unique_ptr<condition>(new player_check(std::move(*this)));
}

const token& player_check::get_player_to_check(void)const{
    return player_to_check;
}

bool player_check::should_be_equal(void)const{
    return equal_check;
}

}

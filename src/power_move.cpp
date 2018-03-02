#include<cassert>

#include"power_move.hpp"
#include"printer_helpers.hpp"

namespace rbg_parser{

power_move::power_move(void):
contained_move(),
number_of_repetitions(1){
}

power_move::power_move(std::unique_ptr<game_move> contained_move, uint number_of_repetitions):
contained_move(std::move(contained_move)),
number_of_repetitions(number_of_repetitions){
}

bool power_move::modifies(void)const{
    if(contained_move)
        return contained_move->modifies();
    else
        return false;
}

std::unique_ptr<game_move> power_move::simplify(void){
    std::unique_ptr<game_move> content = contained_move->simplify();
    if(number_of_repetitions==1)
        return content;
    else
        return std::unique_ptr<game_move>(new power_move(std::move(content), number_of_repetitions));
}

void power_move::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string power_move::print_power(void)const{
    if(number_of_repetitions!=1)
        return "^"+std::to_string(number_of_repetitions);
    else
        return "";
}

std::string power_move::to_rbg(uint indent)const{
    std::string result = "";
    result += open_bracket_if_necessary(priority(),contained_move->priority());
    result += contained_move->to_rbg(indent);
    result += close_bracket_if_necessary(priority(),contained_move->priority());
    result += print_power();
    return result;
}

std::string power_move::to_rbg()const{
    std::string result = "";
    result += open_bracket_if_necessary(priority(),contained_move->priority());
    result += contained_move->to_rbg();
    result += close_bracket_if_necessary(priority(),contained_move->priority());
    result += print_power();
    return result;
}

std::unique_ptr<game_move> power_move::flatten(void){
    return std::unique_ptr<game_move>(new power_move(contained_move->flatten(),number_of_repetitions));
}

straightness_result power_move::compute_k_straightness(void)const{
    auto content_result = contained_move->compute_k_straightness();
    content_result.repeat_result(number_of_repetitions);
    return content_result;
}

bool power_move::finalizer_elligible(void)const{
    return contained_move->finalizer_elligible();
}

const game_move* power_move::get_content(void)const{
    return contained_move.get();
}

uint power_move::get_number_of_repetitions(void)const{
    return number_of_repetitions;
}

}

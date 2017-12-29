#include"pure_bracketed_move.hpp"
#include"printer_helpers.hpp"

pure_bracketed_move::pure_bracketed_move(void):
contained_move(),
number_of_repetitions(1){
}

pure_bracketed_move::pure_bracketed_move(std::unique_ptr<pure_game_move> contained_move, uint number_of_repetitions):
contained_move(std::move(contained_move)),
number_of_repetitions(number_of_repetitions){
}

std::unique_ptr<pure_game_move> pure_bracketed_move::transform_into_pure(void){
    return std::unique_ptr<pure_game_move>(new pure_bracketed_move(std::move(contained_move),number_of_repetitions));
}

std::unique_ptr<pure_game_move> pure_bracketed_move::pure_simplify(void){
    std::unique_ptr<pure_game_move> content = contained_move->pure_simplify();
    if(number_of_repetitions==1)
        return content;
    else
        return std::unique_ptr<pure_game_move>(new pure_bracketed_move(std::move(content), number_of_repetitions));
}

void pure_bracketed_move::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string pure_bracketed_move::print_power(void)const{
    if(number_of_repetitions!=1)
        return "^"+(number_of_repetitions>0 ? std::to_string(number_of_repetitions) : "*");
    return "";
}

std::string pure_bracketed_move::to_rbg(uint indent)const{
    std::string result = "";
    result += open_bracket_if_necessary(priority(),contained_move->priority());
    result += contained_move->to_rbg(indent);
    result += close_bracket_if_necessary(priority(),contained_move->priority());
    result += print_power();
    return result;
}

std::string pure_bracketed_move::to_rbg()const{
    std::string result = "";
    result += open_bracket_if_necessary(priority(),contained_move->priority());
    result += contained_move->to_rbg();
    result += close_bracket_if_necessary(priority(),contained_move->priority());
    result += print_power();
    return result;
}

const pure_game_move* pure_bracketed_move::get_content(void)const{
    return contained_move.get();
}

uint pure_bracketed_move::get_number_of_repetitions(void)const{
    return number_of_repetitions;
}

bool pure_bracketed_move::is_star(void)const{
    return number_of_repetitions==0;
}

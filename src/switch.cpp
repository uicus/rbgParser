#include"switch.hpp"

namespace rbg_parser{

player_switch::player_switch(const token& to_player):
to_player(to_player){
}

int player_switch::give_indices_in_expression(int next_free){
    index = next_free;
    return next_free+1;
}

void player_switch::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string player_switch::to_rbg(const options& opt, uint)const{
    return to_rbg(opt);
}

std::string player_switch::to_rbg(const options&)const{
    return "->" + to_player.to_string() + " ";
}

std::unique_ptr<game_move> player_switch::flatten(void){
    return std::unique_ptr<game_move>(new player_switch(std::move(*this)));
}

straightness_result player_switch::compute_k_straightness([[maybe_unused]] StraightnessType st)const{
    return standard_switch_beginning();
}

const token& player_switch::get_player(void)const{
    return to_player;
}

std::unique_ptr<game_move> player_switch::copy(void)const{
    return std::unique_ptr<game_move>(new player_switch(to_player));
}

bool player_switch::is_modifier(void)const{
    return true;
}

keeper_switch::keeper_switch(bool deterministic):
deterministic(deterministic){
}

int keeper_switch::give_indices_in_expression(int next_free){
    index = next_free;
    return next_free+1;
}

void keeper_switch::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string keeper_switch::to_rbg(const options& opt, uint)const{
    return to_rbg(opt);
}

std::string keeper_switch::to_rbg(const options&)const{
    return is_deterministic() ? "->>" : "->*";
}

std::unique_ptr<game_move> keeper_switch::flatten(void){
    return std::unique_ptr<game_move>(new keeper_switch(std::move(*this)));
}

straightness_result keeper_switch::compute_k_straightness(StraightnessType st)const{
    switch (st) {
    case StraightnessType::APP_STRAIGHTNESS:
    case StraightnessType::MAIN_STRAIGHTNESS:
        return standard_switch_beginning();
    default:
        return standard_switch_nonbeginning();
    }
}

bool keeper_switch::is_deterministic(void)const{
    return deterministic;
}

std::unique_ptr<game_move> keeper_switch::copy(void)const{
    return std::unique_ptr<game_move>(new keeper_switch(deterministic));
}

bool keeper_switch::is_modifier(void)const{
    return true;
}

}

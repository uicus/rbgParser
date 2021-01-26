#include"assignments.hpp"
#include"printer_helpers.hpp"
#include"concatenation.hpp"

namespace rbg_parser{

assignment::assignment(const token& left_side, std::unique_ptr<arithmetic_expression>&& right_side):
left_side(left_side),
right_side(std::move(right_side)){
}

int assignment::give_indices_in_expression(int next_free){
    index = next_free;
    return next_free+1;
}

void assignment::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string assignment::to_rbg(const options& opt, uint)const{
    return to_rbg(opt);
}

std::string assignment::to_rbg(const options& opt)const{
    std::string result = "";
    if(opt.enabled_noop_before_modifier())
        result += ".";
    result += "[$ ";
    result += print_variable(left_side);
    result += "=";
    result += right_side->to_rbg();
    result += "]";
    if(opt.enabled_noop_after_modifier())
        result += ".";
    return result;
}

std::unique_ptr<game_move> assignment::simplify(void){
    auto new_right_side = right_side->simplify();
    return std::unique_ptr<game_move>(new assignment(std::move(left_side), std::move(new_right_side)));
}

std::unique_ptr<game_move> assignment::flatten(void){
    auto new_right_side = right_side->flatten();
    return std::unique_ptr<game_move>(new assignment(std::move(left_side), std::move(new_right_side)));
}

straightness_result assignment::compute_k_straightness([[maybe_unused]] StraightnessType st)const{
    return modifier_non_switch();
}

bool assignment::check_if_redundant(std::set<token>& assignments_so_far, bool&)const{
    if(assignments_so_far.find(left_side) != assignments_so_far.end())
        return true;
    assignments_so_far.insert(left_side);
    return false;
}

const token& assignment::get_left_side(void)const{
    return left_side;
}

const arithmetic_expression* assignment::get_right_side(void)const{
    return right_side.get();
}

std::unique_ptr<game_move> make_assignments_concatenation(
    std::vector<std::pair<token, std::unique_ptr<arithmetic_expression>>>&& legal_sides){
    concatenation* result = new concatenation;
    for(auto& el: legal_sides)
        result->add_move(std::unique_ptr<game_move>(new assignment(std::move(el.first), std::move(el.second))));
    return std::unique_ptr<game_move>(result);
}

std::unique_ptr<game_move> assignment::copy(void)const{
    return std::unique_ptr<game_move>(new assignment(left_side, right_side->copy()));
}

bool assignment::has_piece_as_variable(const std::set<token>& pieces)const{
    return right_side->has_piece_as_variable(pieces);
}

bool assignment::is_modifier(void)const{
    return true;
}

}

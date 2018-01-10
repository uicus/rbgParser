#include"assignments.hpp"
#include"printer_helpers.hpp"

namespace rbg_parser{

assignment::assignment(token&& left_side, token&& right_side,bool lazy):
left_side(std::move(left_side)),
right_side(std::move(right_side)),
lazy(lazy){
}

void assignment::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string assignment::to_rbg(uint)const{
    return to_rbg();
}

std::string assignment::to_rbg()const{
    std::string result = "";
    result += (lazy ? "[@" : "[");
    result += print_variable(left_side);
    result += "=";
    result += (left_side.get_type() == player ? right_side.to_string() : print_variable(right_side));
    result += "]";
    return result;
}

std::unique_ptr<game_move> assignment::flatten(void){
    return std::unique_ptr<game_move>(new assignment(std::move(*this)));
}

void assignment::gather_concatenation_elements(
    std::vector<std::unique_ptr<game_move>>&,
    std::vector<std::unique_ptr<game_move>>& next_block_elements){
    next_block_elements.push_back(flatten());
}

straightness_result assignment::compute_k_straightness(void)const{
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

const token& assignment::get_right_side(void)const{
    return right_side;
}

bool assignment::is_lazy(void)const{
    return lazy;
}

parser_result<token> parse_left_side_variable(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing variable");
    if(!it.has_value() || it.current(msg).get_type()!=dollar)
        return failure<token>();
    it.next(msg);
    if(it.current(msg).get_type()!=identifier)
        throw msg.build_message(it.create_call_stack("Expected variable, encountered \'"+it.current(msg).to_string()+"\'"));
    if(decls.get_legal_variables().count(it.current(msg))==0 && decls.get_legal_players().count(it.current(msg))==0)
        throw msg.build_message(it.create_call_stack("Undeclared variable or player \'"+it.current(msg).to_string()+"\'"));
    token result = it.current(msg);
    it.next(msg);
    return success(std::move(result));
}

parser_result<assignment> parse_single_assignment(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    auto variable_result = parse_left_side_variable(it,decls,msg);
    if(!variable_result.is_success())
        return failure<assignment>();
    if(it.current(msg).get_type() != equal)
        throw msg.build_message(it.create_call_stack("Expected \'=\' after variable name, encountered \'"+it.current(msg).to_string()+"\'"));
    it.next(msg);
    if(it.current(msg).get_type() != number)
        throw msg.build_message(it.create_call_stack("Expected non-negative number after variable name, encountered \'"+it.current(msg).to_string()+"\'"));
    token num = it.current(msg);
    it.next(msg);
    return success(assignment(variable_result.move_value(),std::move(num)));
}

parser_result<concatenation> parse_assignments(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing assignemnts");
    concatenation result;
    auto assignment_result = parse_single_assignment(it,decls,msg);
    if(!assignment_result.is_success())
        return failure<concatenation>();
    result.add_move(std::unique_ptr<game_move>(new assignment(assignment_result.move_value())));
    while(it.has_value() && it.current(msg).get_type() == comma){
        it.next(msg);
        auto assignment_result = parse_single_assignment(it,decls,msg);
        if(!assignment_result.is_success())
            throw msg.build_message(it.create_call_stack("Expected variable assignment after comma, encountered \'"+it.current(msg).to_string()+"\'"));
        result.add_move(std::unique_ptr<game_move>(new assignment(assignment_result.move_value())));
    }
    return success(std::move(result));
}

}

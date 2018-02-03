#include"assignments.hpp"
#include"printer_helpers.hpp"

namespace rbg_parser{

assignment::assignment(token&& left_side, token&& right_side, token&& kind_of_modification, bool lazy):
left_side(std::move(left_side)),
right_side(std::move(right_side)),
kind_of_modification(std::move(kind_of_modification)),
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
    result += kind_of_modification.to_string();
    if(is_assignment())
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

bool assignment::finalizer_elligible(void)const{
    return true;
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

bool assignment::is_assignment(void)const{
    return kind_of_modification.get_type() == equal;
}

bool assignment::is_incrementation(void)const{
    return kind_of_modification.get_type() == double_plus;
}

bool assignment::is_decrementation(void)const{
    return kind_of_modification.get_type() == double_minus;
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
    token kind_of_action = it.current(msg);
    if(kind_of_action.get_type() != equal && kind_of_action.get_type() != double_plus && kind_of_action.get_type() != double_minus)
        throw msg.build_message(it.create_call_stack("Expected \'=\', \'++\' or \'--\' after variable name, encountered \'"+it.current(msg).to_string()+"\'"));
    token num;
    if(kind_of_action.get_type() == equal){
        it.next(msg);
        if(it.current(msg).get_type() == number)
            num = it.current(msg);
        else if(it.current(msg).get_type() == dollar){
            it.next(msg);
            num = it.current(msg);
            if(num.get_type()!=identifier)
                throw msg.build_message(it.create_call_stack("Expected identifier after \'$\', encountered \'"+it.current(msg).to_string()+"\'"));
            if(decls.get_legal_pieces().count(num)==0 && decls.get_legal_players().count(num)==0 && decls.get_legal_variables().count(num)==0)
                throw msg.build_message(it.create_call_stack("Variable, piece or player \'"+num.to_string()+"\' was not declared"));
        }
        else
            throw msg.build_message(it.create_call_stack("Expected non-negative number or variable after \'=\', encountered \'"+it.current(msg).to_string()+"\'"));
    }
    it.next(msg);
    return success(assignment(variable_result.move_value(),std::move(num),std::move(kind_of_action)));
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

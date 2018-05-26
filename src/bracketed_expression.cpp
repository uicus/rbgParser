#include"bracketed_expression.hpp"
#include"typing_machine.hpp"
#include"arithmetic_expression.hpp"
#include"condition_check.hpp"
#include"assignments.hpp"
#include"offs.hpp"
#include"ons.hpp"
#include"shift.hpp"
#include"power_move.hpp"
#include"star_move.hpp"
#include"conditional_star_move.hpp"

namespace rbg_parser{

bracketed_expression::bracketed_expression(
    slice_iterator&& beginning_position,
    std::unique_ptr<expression> element,
    bracket_type br):
expression(std::move(beginning_position)),
element(std::move(element)),
br(br),
t(not_typed_yet){
}

std::unique_ptr<game_move> bracketed_expression::append_suffix_if_possible(std::unique_ptr<game_move> base_move)const{
    switch(s.t){
        case number_power:
            return std::unique_ptr<game_move>(new power_move(std::move(base_move), s.val));
        case star_power:
            return std::unique_ptr<game_move>(new star_move(std::move(base_move)));
        case conditional_star_power:
            return std::unique_ptr<game_move>(new conditional_star_move(std::move(base_move)));
        case no_suffix:
        default:
            return std::move(base_move);
    }
}

expression_type bracketed_expression::get_type(void)const{
    return t;
}

void bracketed_expression::type(const typing_machine& m, messages_container& msg){
    if(t != not_typed_yet)
        return;
    element->type(m, msg);
    auto result = m.evaluate_brackets(br, element->get_type(), s.t);
    if(result == error_type){
        std::string error_message = bracket_type_description(br)+" cannot be applied to ";
        error_message += expression_type_description(element->get_type());
        error_message += " (with ";
        error_message += suffix_type_description(s.t) + ")";
        throw msg.build_message(beginning_position.create_call_stack(error_message));
    }
    t = result;
}

std::unique_ptr<arithmetic_expression> bracketed_expression::get_arithmetic_expression(void)const{
    assert(t == arithmetics);
    return element->get_arithmetic_expression();
}

std::unique_ptr<game_move> bracketed_expression::get_game_move(void)const{
    switch(t){
        case gcondition:
        switch(br){
            case condition_bracket:
                return std::unique_ptr<game_move>(new condition_check(element->get_condition(), false));
            case negated_condition_bracket:
                return std::unique_ptr<game_move>(new condition_check(element->get_condition(), true));
            default:
                assert(false);
        }
        case assignments_move:
        {
            auto result = make_assignments_concatenation(element->get_assignments_sequence());
            return std::move(result);
        }
        case offs_move:
        {
            auto result = make_offs_sum(element->get_identifiers_sequence());
            return std::move(result);
        }
        case on_move:
            return std::unique_ptr<game_move>(new ons(element->get_identifiers_sequence()));
        case gmove:
            return append_suffix_if_possible(element->get_game_move());
        default:
            assert(false);
    }
}

std::unique_ptr<condition> bracketed_expression::get_condition(void)const{
    return get_game_move();
}

}

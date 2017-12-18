#include<cassert>

#include"bracketed_move.hpp"
#include"shift.hpp"
#include"ons.hpp"
#include"sum.hpp"
#include"pure_bracketed_move.hpp"
#include"condition_check.hpp"
#include"offs.hpp"
#include"assignments.hpp"
#include"switch.hpp"

bracketed_move::bracketed_move(void):
contained_move(),
number_of_repetitions(1){
}

bracketed_move::bracketed_move(std::unique_ptr<game_move> contained_move, uint number_of_repetitions):
contained_move(std::move(contained_move)),
number_of_repetitions(number_of_repetitions){
}

bool bracketed_move::modifies(void){
    if(contained_move)
        return contained_move->modifies();
    else
        return false;
}

std::unique_ptr<pure_game_move> bracketed_move::transform_into_pure(void){
    return std::unique_ptr<pure_game_move>(new pure_bracketed_move(contained_move->transform_into_pure(),number_of_repetitions));
}

void bracketed_move::accept(abstract_dispatcher& dispatcher){
    dispatcher.dispatch(*this);
}

const game_move* bracketed_move::get_content(void)const{
    return contained_move.get();
}

uint bracketed_move::get_number_of_repetitions(void)const{
    return number_of_repetitions;
}

bool bracketed_move::is_star(void)const{
    return number_of_repetitions == 0;
}

parser_result<bracketed_move> parse_non_modifier(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    std::unique_ptr<game_move> contained_move;
    auto shift_result = parse_shift(it,msg);
    if(shift_result.is_success())
        contained_move = std::unique_ptr<game_move>(new shift(shift_result.move_value()));
    // due to specific syntax interpretation another options should go here and ons should be last
    else{
    auto condition_result = parse_condition_check(it,decls,msg);
    if(condition_result.is_success())
        contained_move = std::unique_ptr<game_move>(new condition_check(condition_result.move_value()));
    else{
    auto sum_result = parse_sum(it,decls,msg);
    if(sum_result.is_success())
        contained_move = std::unique_ptr<game_move>(new sum(sum_result.move_value()));
    else{
    auto ons_result = parse_ons(it,decls,msg);
    if(ons_result.is_success())
        contained_move = std::unique_ptr<game_move>(new ons(ons_result.move_value()));
    else{
        assert(false);
    }
    }
    }
    }
    if(it.current(msg).get_type() != right_round_bracket)
        throw msg.build_message(it.create_call_stack("Expected \')\', encountered \'"+it.current(msg).to_string()+"\'"));
    if(!it.next(msg) || it.current(msg).get_type() != caret)
        return success(bracketed_move(std::move(contained_move)));
    it.next(msg);
    if(it.current(msg).get_type() == star){
        it.next(msg);
        return success(bracketed_move(std::move(contained_move),0));
    }
    auto before_int_it = it;
    auto number_result = parse_int(it,msg);
    if(!number_result.is_success())
        throw msg.build_message(it.create_call_stack("Expected \'*\' or number after \'^\', encountered \'"+it.current(msg).to_string()+"\'"));
    if(number_result.get_value()<1)
        throw msg.build_message(before_int_it.create_call_stack("Number after \'^\' should be positive"));
    return success(bracketed_move(std::move(contained_move),number_result.get_value()));
}

parser_result<bracketed_move> parse_modifier(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    std::unique_ptr<game_move> contained_move;
    bool lazy = false;
    if(it.current(msg).get_type() == at_sign){
        lazy = true;
        it.next(msg);
        if(!it.has_value())
            throw msg.build_message(it.create_call_stack("Unexpected end of input after \'@\'"));
    }
    auto assignments_result = parse_assignments(it,decls,msg);
    if(assignments_result.is_success())
        contained_move = std::unique_ptr<game_move>(new concatenation(assignments_result.move_value()));
    else{
    auto offs_result = parse_offs(it,decls,msg);
    if(offs_result.is_success())
        contained_move = std::unique_ptr<game_move>(new sum(offs_result.move_value()));
    else
        assert(false);
    }
    if(it.current(msg).get_type() != right_square_bracket)
        throw msg.build_message(it.create_call_stack("Expected \']\', encountered \'"+it.current(msg).to_string()+"\'"));
    if(lazy)
        contained_move->set_lazy();
    it.next(msg);
    return success(bracketed_move(std::move(contained_move)));
}

parser_result<bracketed_move> parse_bracketed_move(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing brackets around the move");
    if(!it.has_value())
        return failure<bracketed_move>();
    if(it.current(msg).get_type() == left_round_bracket){
        it.next(msg);
        return parse_non_modifier(it,decls,msg);
    }
    else if(it.current(msg).get_type() == left_square_bracket){
        it.next(msg);
        return parse_modifier(it,decls,msg);
    }
    else if(it.current(msg).get_type() == arrow){
        auto switch_result = parse_player_switch(it,decls,msg);
        assert(switch_result.is_success());
        return success(bracketed_move(std::unique_ptr<game_move>(new player_switch(switch_result.move_value()))));
    }
    else
        return failure<bracketed_move>();
}

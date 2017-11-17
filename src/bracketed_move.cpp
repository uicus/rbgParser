#include<cassert>

#include"bracketed_move.hpp"
#include"shift.hpp"
#include"ons.hpp"

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

parser_result<bracketed_move> parse_bracketed_move(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing brackets around the move");
    if(it.current(msg).get_type() != left_round_bracket)
        return failure<bracketed_move>();
    if(it.next(msg))
        throw msg.build_message("Unexpected end of input after \'(\'");
    std::unique_ptr<game_move> contained_move;
    auto shift_result = parse_shift(it,msg);
    if(shift_result.is_success())
        contained_move = std::unique_ptr<game_move>(new shift(shift_result.move_value()));
    // another options should go here, as ons should be last
    else{
    auto ons_result = parse_ons(it,decls,msg);
    if(ons_result.is_success())
        contained_move = std::unique_ptr<game_move>(new ons(ons_result.move_value()));
    else{
        assert(false); // I'm sure we cannot reach this point
    }
    }
    if(it.current(msg).get_type() != right_round_bracket)
        throw msg.build_message(it.create_call_stack("Expected \')\', encountered \'"+it.current(msg).to_string()+"\'"));
    if(!it.next(msg) || it.current(msg).get_type() != caret)
        return success(bracketed_move(std::move(contained_move)));
    if(!it.next(msg))
        throw msg.build_message("Unexpected end of input after \'^\'");
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

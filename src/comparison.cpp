#include"comparison.hpp"
#include"printer_helpers.hpp"

namespace rbg_parser{

comparison::comparison(token left_side,token kind_of_comparison,token right_side):
left_side(left_side),
kind_of_comparison(kind_of_comparison),
right_side(right_side){
    sort();
}

void comparison::sort(void){
    if(left_side<right_side){
        reverse();
    }
}

void comparison::reverse(void){
    std::swap(left_side,right_side);
    kind_of_comparison.reverse_comparison();
}

void comparison::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string comparison::to_rbg(uint)const{
    return to_rbg();
}

std::string comparison::to_rbg()const{
    std::string result = "";
    result += print_variable(left_side);
    result += kind_of_comparison.to_string();
    result += (left_side.get_type() == player ? right_side.to_string() : print_variable(right_side));
    return result;
}

std::unique_ptr<condition> comparison::flatten(void){
    return std::unique_ptr<condition>(new comparison(std::move(*this)));
}

const token& comparison::get_left_side(void)const{
    return left_side;
}

const token& comparison::get_kind_of_comparison(void)const{
    return kind_of_comparison;
}

const token& comparison::get_right_side(void)const{
    return right_side;
}

// true if variable was naked (e.g. white in $player == white)
parser_result<std::pair<token,bool>> parse_side(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    if(!it.has_value())
        return failure<std::pair<token,bool>>();
    if(it.current(msg).get_type() == dollar){
        it.next(msg);
        if(it.current(msg).get_type()!=identifier && it.current(msg).get_type()!=player && it.current(msg).get_type()!=turn)
            throw msg.build_message(it.create_call_stack("Expected variable, \'turn\' or \'player\' token, encountered \'"+it.current(msg).to_string()+"\'"));
        token name = it.current(msg);
        auto ident_pos = it;
        it.next(msg);
        if(name.get_type()==player || name.get_type()==turn)
            return success(std::make_pair(std::move(name),false));
        if(decls.get_legal_pieces().count(name)==0 && decls.get_legal_players().count(name)==0 && decls.get_legal_variables().count(name)==0)
            throw msg.build_message(ident_pos.create_call_stack("Variable, piece or player \'"+ident_pos.current(msg).to_string()+"\' was not declared"));
        return success(std::make_pair(std::move(name),false));
    }
    else if(it.current(msg).get_type()==number){
        token n = it.current(msg);
        it.next(msg);
        return success(std::make_pair(std::move(n),false));
    }
    else if(it.current(msg).get_type()==identifier){
        token pl = it.current(msg);
        if(decls.get_legal_players().count(pl)==0)
            return failure<std::pair<token,bool>>();
        it.next(msg);
        return success(std::make_pair(std::move(pl),true));
    }
    else
        return failure<std::pair<token,bool>>();
}

parser_result<comparison> parse_comparison(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing comparison");
    if(!it.has_value())
        return failure<comparison>();
    auto beginning = it;
    auto left_side = parse_side(it,decls,msg);
    if(!left_side.is_success())
        return failure<comparison>();
    if(it.current(msg).get_type() != less &&
       it.current(msg).get_type() != less_equal &&
       it.current(msg).get_type() != greater &&
       it.current(msg).get_type() != greater_equal &&
       it.current(msg).get_type() != double_equal &&
       it.current(msg).get_type() != not_equal)
        throw msg.build_message(it.create_call_stack("Expected comparison operator, encountered \'"+it.current(msg).to_string()+"\'"));
    token kind_of_comparison = it.current(msg);
    it.next(msg);
    auto right_side = parse_side(it,decls,msg);
    if(!right_side.is_success())
        throw msg.build_message(it.create_call_stack("Expected variable or constant, encountered \'"+it.current(msg).to_string()+"\'"));
    comparison result(left_side.get_value().first,kind_of_comparison,right_side.get_value().first);
    if(result.left_side.get_type()==player){
        if(!left_side.get_value().second && !right_side.get_value().second)
            throw msg.build_message(beginning.create_call_stack("Variable \'$player\' can only be compared to player names"));
        if(kind_of_comparison.get_type()!=not_equal && kind_of_comparison.get_type()!=double_equal)
            throw msg.build_message(beginning.create_call_stack("Variable \'$player\' can only be check for equality"));
    }
    else{
        if(left_side.get_value().second || right_side.get_value().second)
            throw msg.build_message(beginning.create_call_stack("Player name in non-$player comparison"));
    }
    return success(std::move(result));
}

}

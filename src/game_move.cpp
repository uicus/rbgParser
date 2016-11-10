#include"game_move.hpp"

atomic_move::atomic_move(void):
x(0),
y(0),
every_on_legal(false),
on(),
off(){}

atomic_move::atomic_move(int _x,int _y,const std::set<token>& _on,const std::set<token>& _off):
x(_x),
y(_y),
every_on_legal(false),
on(_on),
off(_off){}

atomic_move::atomic_move(int _x,int _y,const std::set<token>& _off):
x(_x),
y(_y),
every_on_legal(true),
on(),
off(_off){}

parser_result<atomic_move> parse_atomic_move(slice_iterator& it, messages_container& msg)throw(message){
    if(!it.has_value())
        return failure<atomic_move>();
    if(!it.current().get_type() != left_round_bracket)
        return failure<atomic_move>();
    if(!it.next(msg))
        throw msg.build_message("Unexpected end of atomic move");
    int x=0,y=0;
    parser_result<int> delta = parse_int(it,msg);
    if(delta.is_success()){
        x = delta.get_value();
        if(!it.has_value())
            throw msg.build_message("Unexpected end of atomic move");
        if(it.current().get_type() != comma)
            throw msg.build_message(it.create_call_stack("Expected ',', encountered \'"+it.current().to_string()+"\'"));
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of atomic move");
        delta = parse_int(it,msg);
        if(delta.is_success())
            y = delta.get_value();
        else
            throw msg.build_message(it.create_call_stack("Expected integer, encountered \'"+it.current().to_string()+"\'"));
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of atomic move");
        if(!it.current().get_type() != comma)
            throw msg.build_message(it.create_call_stack("Expected ',', encountered \'"+it.current().to_string()+"\'"));
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of atomic move");
    }
    bool every_on_legal = false;
    std::set<token> on,off;
    if(it.current().get_type() == slash){
        every_on_legal = true;
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of atomic move");
        parser_result<std::set<token>> legals = parse_tokens_set(it,msg);
        if(!legals.is_success())
            off = legals.get_value();
        else
            throw msg.build_message(it.create_call_stack("Expected set of identifiers, encountered \'"+it.current().to_string()+"\'"));
    }
    else{
        parser_result<std::set<token>> legals = parse_tokens_set(it,msg);
        if(legals.is_success())
            on = legals.get_value();
        else
            throw msg.build_message(it.create_call_stack("Expected set of identifiers or '/', encountered \'"+it.current().to_string()+"\'"));
        if(!it.has_value())
            throw msg.build_message("Unexpected end of atomic move");
        if(it.current().get_type() == slash){
            if(!it.next(msg))
                throw msg.build_message("Unexpected end of atomic move");
            legals = parse_tokens_set(it,msg);
            if(!legals.is_success())
                off = legals.get_value();
            else
                throw msg.build_message(it.create_call_stack("Expected set of identifiers, encountered \'"+it.current().to_string()+"\'"));
        }
        else
            off = on;
    }
    if(!it.has_value())
        throw msg.build_message("Unexpected end of atomic move");
    if(!it.current().get_type() != right_round_bracket)
        throw msg.build_message(it.create_call_stack("Expected ')' at the end of atomic move, encountered \'"+it.current().to_string()+"\'"));
    return success(every_on_legal ? atomic_move(x,y,off) : atomic_move(x,y,on,off));
}

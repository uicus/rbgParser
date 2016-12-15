#include"parser_helpers.hpp"
#include"types.hpp"

parser_result<int> parse_int(slice_iterator& it, messages_container& msg)throw(message){
    bool is_positive = true;
    if(!it.has_value())
        return failure<int>();
    if(it.current().get_type() == plus)
        it.next(msg);
    else if(it.current().get_type() == minus){
        is_positive = false;
        it.next(msg);
    }
    else if(it.current().get_type() != number)
        return failure<int>();
    if(!it.has_value())
        throw msg.build_message("Unexpected end of number");
    if(it.current().get_type() != number)
        throw msg.build_message(it.create_call_stack("Expected digit, encountered \'"+it.current().to_string()+"\'"));
    int result = is_positive ? int(it.current().get_value()) : -int(it.current().get_value());
    it.next(msg);
    return success(std::move(result));
}

parser_result<std::set<token>> parse_tokens_set(slice_iterator& it, messages_container& msg)throw(message){
    std::set<token> result;
    if(!it.has_value())
        return failure<std::set<token>>();
    if(it.current().get_type() == identifier)
        result.insert(it.current());
    else if(it.current().get_type() == left_curly_bracket){
        if(!it.next(msg))
            throw msg.build_message("Not terminated tokens set");
        if(it.current().get_type() == right_curly_bracket); // empty set
        else while(true){
            if(it.current().get_type() != identifier)
                throw msg.build_message(it.create_call_stack("Expected identifier inside set, encountered \'"+it.current().to_string()+"\'"));
            result.insert(it.current());
            if(!it.next(msg))
                throw msg.build_message("Not terminated tokens set");
            if(it.current().get_type() == right_curly_bracket)
                break;
            if(it.current().get_type() != comma)
                throw msg.build_message(it.create_call_stack("Expected ',' or '}', encountered \'"+it.current().to_string()+"\'"));
            if(!it.next(msg))
                throw msg.build_message("Not terminated tokens set");
        };
    }
    else
        return failure<std::set<token>>();
    it.next(msg);
    return success(std::move(result));
}

parser_result<token> parse_variable(slice_iterator& it, const std::set<token>& encountered_pieces, messages_container& msg)throw(message){
    if(!it.has_value())
        return failure<token>();
    if(it.current().get_type()!=dollar)
        return failure<token>();
    if(!it.next(msg))
        throw msg.build_message("Unexpected end of variable");
    if(it.current().get_type()!=identifier)
        throw msg.build_message(it.create_call_stack("Expected identifier, encountered \'"+it.current().to_string()+"\'"));
    if(encountered_pieces.count(it.current())==0&&it.current().to_string()!="turn")
        throw msg.build_message(it.create_call_stack("There's no piece \'"+it.current().to_string()+"\'"));
    token result = it.current();
    it.next(msg);
    return success(std::move(result));
}

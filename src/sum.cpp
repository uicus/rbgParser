#include"sum.hpp"
#include"concatenation.hpp"
#include"pure_sum.hpp"

sum::sum(std::vector<std::unique_ptr<game_move>>&& content):
content(std::move(content)){
}

bool sum::modifies(void){
    for(const auto& el: content)
        if(el->modifies())
            return true;
    return false;
}

std::unique_ptr<pure_game_move> sum::transform_into_pure(void){
    std::vector<std::unique_ptr<pure_game_move>> result;
    for(uint i=0;i<content.size();++i)
        result.push_back(content[i]->transform_into_pure());
    return std::unique_ptr<pure_game_move>(new pure_sum(std::move(result)));
}

void sum::add_move(std::unique_ptr<game_move> m){
    content.push_back(std::move(m));
}

parser_result<sum> parse_sum(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing moves sum");
    std::vector<std::unique_ptr<game_move>> result;
    bool already_started = false;
    while(true){
        if(already_started){
            if(!it.has_value() || it.current(msg).get_type() != plus)
                return success(sum(std::move(result)));
            else
                it.next(msg);
        }
        auto lower_result = parse_concatenation(it,decls,msg);
        if(!lower_result.is_success()){
            if(already_started)
                throw msg.build_message(it.create_call_stack("Expected moves concatenation, encountered \'"+it.current(msg).to_string()+"\'"));
            else
                return failure<sum>();
        }
        auto next_elem = std::unique_ptr<game_move>(new concatenation(lower_result.move_value()));
        result.push_back(std::move(next_elem));
        already_started = true;
    }
}

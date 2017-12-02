#include"concatenation.hpp"
#include"bracketed_move.hpp"

concatenation::concatenation(std::vector<std::unique_ptr<game_move>>&& content):
content(std::move(content)){
}

bool concatenation::modifies(void){
    for(const auto& el: content)
        if(el->modifies())
            return true;
    return false;
}

parser_result<concatenation> parse_concatenation(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing moves concatenation");
    std::vector<std::unique_ptr<game_move>> result;
    while(true){
        auto lower_result = parse_bracketed_move(it,decls,msg);
        if(!lower_result.is_success()){
            if(result.empty())
                return failure<concatenation>();
            else
                return success(concatenation(std::move(result)));
        }
        auto next_elem = std::unique_ptr<game_move>(new bracketed_move(lower_result.move_value()));
        result.push_back(std::move(next_elem));
    }
}

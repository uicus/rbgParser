#include"offs.hpp"

namespace rbg_parser{

off::off(token&& off_piece, bool lazy):
off_piece(std::move(off_piece)),
lazy(lazy){
}

void off::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

const token& off::get_piece(void)const{
    return off_piece;
}

bool off::is_lazy(void)const{
    return lazy;
}

std::string off::to_rbg(uint)const{
    return to_rbg();
}

std::string off::to_rbg()const{
    std::string result = "";
    result += (lazy ? "[@" : "[");
    result += off_piece.to_string();
    result += "]";
    return result;
}

parser_result<sum> parse_offs(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message){
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing offs");
    auto set_result = parse_sequence(it,"legal offs",decls.get_legal_pieces(),true,msg);
    if(!set_result.is_success())
        return failure<sum>();
    else{
        sum result;
        std::vector<token> temp_token_set(set_result.move_value());
        while(!temp_token_set.empty()){
            result.add_move(std::unique_ptr<game_move>(new off(std::move(temp_token_set.back()))));
            temp_token_set.pop_back();
        }
        return success(std::move(result));
    }
}

}

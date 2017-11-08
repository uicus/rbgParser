#include<cassert>

#include"game_items.hpp"
#include"types.hpp"
#include"parser_helpers.hpp"

game_items::game_items(void)noexcept:
macros(),
game_segment(nullptr),
board_segment(nullptr),
players_segment(nullptr),
variables_segment(nullptr),
pieces_segment(nullptr),
rules_segment(nullptr),
next_item_context_order(0){
}

game_items::game_items(game_items&& src)noexcept:
macros(std::move(src.macros)),
game_segment(src.game_segment),
board_segment(src.board_segment),
players_segment(src.players_segment),
variables_segment(src.variables_segment),
pieces_segment(src.pieces_segment),
rules_segment(src.rules_segment),
next_item_context_order(src.next_item_context_order){
    src.game_segment = src.board_segment = src.players_segment = src.variables_segment = src.pieces_segment = src.rules_segment = nullptr;
}

game_items& game_items::operator=(game_items&& src)noexcept{
    if(this != &src){
        macro_bank temp(std::move(src.macros));
        src.macros = std::move(macros);
        macros = std::move(temp);
        std::swap(game_segment,src.game_segment);
        std::swap(board_segment,src.board_segment);
        std::swap(players_segment,src.players_segment);
        std::swap(variables_segment,src.variables_segment);
        std::swap(pieces_segment,src.pieces_segment);
        std::swap(rules_segment,src.rules_segment);
        next_item_context_order = src.next_item_context_order;
    }
    return *this;
}

game_items::~game_items(void)noexcept{
    delete game_segment;
    delete board_segment;
    delete players_segment;
    delete variables_segment;
    delete pieces_segment;
    delete rules_segment;
}

uint game_items::input_macro(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message){
    if(current_token >= input.size() || input[current_token].get_type() == hash)
        throw msg.build_message(input[current_token-1].get_position(),"Empty macro directive");
    if(input[current_token].get_type() != identifier)
        throw msg.build_message(input[current_token].get_position(),"Expected identifier, encountered \'"+input[current_token].to_string()+"\'");
    token name = input[current_token];
    std::vector<token> args;
    current_token = parse_arguments(input,current_token+1,args,msg);
    if((args.size()>0 && macros.is_zero_arity_macro(name,next_item_context_order)) || (args.size()==0 && macros.is_non_zero_arity_macro(name,next_item_context_order)))
        throw msg.build_message(name.get_position(),"Cannot define two macros with the same name with zero and non zero arities");
    if(macros.is_macro_name_and_arity(name,args.size(),next_item_context_order))
        msg.add_message(name.get_position(),"Macro \'"+name.to_string()+"\' with arity "+std::to_string(args.size())+" already defined, overwriting previous definition");
    if(current_token >= input.size() || input[current_token].get_type() == hash)
        throw msg.build_message(input[current_token-1].get_position(),"Unexpected end of macro definition");
    if(input[current_token].get_type() != equal)
        throw msg.build_message(input[current_token].get_position(),"Expected \'=\' token, encountered \'"+input[current_token].to_string()+"\'");
    uint begin = current_token+1;
    uint end = current_token = reach_end_of_directive(input,begin);
    macro m = macro(std::move(args),&input,begin,end,next_item_context_order++);
    if(m.is_empty())
        msg.add_message(name.get_position(),"Macro \'"+name.to_string()+"\' doesn't have body");
    macros.add_macro(std::move(name),std::move(m));
    return current_token;
}

uint game_items::input_slice(
const std::vector<token>& input,
uint current_token,
const std::string& segment_name,
slice* game_items::*segment_position,
bool should_be_nonempty,
messages_container& msg)throw(message){
    if(current_token >= input.size())
        throw msg.build_message(input[current_token-1].get_position(),"Unexpected end of \'"+segment_name+"\' segment");
    if(input[current_token].get_type() != equal)
        throw msg.build_message(input[current_token].get_position(),"Expected \'=\' after \'"+segment_name+"\' segment name, encountered \'"+input[current_token].to_string()+"\'");
    ++current_token;
    uint begin = current_token;
    uint end = current_token = reach_end_of_directive(input,current_token);
    slice* s = new slice(&input,begin,end,next_item_context_order++);
    if(s->is_empty() && should_be_nonempty)
        msg.add_message(input[begin-1].get_position(),"Empty \'"+segment_name+"\' directive");
    this->*segment_position = s;
    return end;
}

uint game_items::input_game(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message){
    return input_slice(input, current_token, "game", &game_items::game_segment,true,msg);
}

uint game_items::input_board(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message){
    return input_slice(input, current_token, "board", &game_items::board_segment,true,msg);
}

uint game_items::input_players(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message){
    return input_slice(input, current_token, "players", &game_items::players_segment,true,msg);
}

uint game_items::input_variables(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message){
    return input_slice(input, current_token, "variables", &game_items::variables_segment,false,msg);
}

uint game_items::input_pieces(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message){
    return input_slice(input, current_token, "pieces", &game_items::pieces_segment,true,msg);
}

uint game_items::input_rules(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message){
    return input_slice(input, current_token, "rules", &game_items::rules_segment,true,msg);
}

uint reach_end_of_directive(const std::vector<token>& input,uint current_token){
    while(current_token < input.size() && input[current_token].get_type() != hash)
        ++current_token;
    return current_token;
}

uint parse_arguments(const std::vector<token>& input,uint current_token,std::vector<token>& args,messages_container& msg)throw(message){
    if(input[current_token].get_type() != left_round_bracket || current_token >= input.size())
        return current_token;
    while(true){
        ++current_token;
        if(current_token >= input.size() || input[current_token].get_type() == hash)
            throw msg.build_message(input[current_token-1].get_position(),"Unexpected end of macro arguments list");
        if(input[current_token].get_type() != identifier)
            throw msg.build_message(input[current_token].get_position(),"Expected identifier, encountered \'"+input[current_token].to_string()+"\'");
        args.push_back(input[current_token++]);
        if(current_token >= input.size() || input[current_token].get_type() == hash)
            throw msg.build_message(input[current_token-1].get_position(),"Unexpected end of macro arguments list");
        if(input[current_token].get_type() == right_round_bracket)
            return current_token+1;
        if(input[current_token].get_type() != comma)
            throw msg.build_message(input[current_token].get_position(),"Expected \',\' or \'(\' token, encounered \'"+input[current_token].to_string()+"\'");
    }
}

game_items input_tokens(const std::vector<token>& input,messages_container& msg)throw(message){
    game_items result;
    if(input.empty())
        throw msg.build_message("No tokens in file");
    uint current_token = 0;
    if(input[current_token].get_type() != hash)
        throw msg.build_message(input[current_token].get_position(),"Expected \'#\' token at beginning of file, encountered \'"+input[current_token].to_string()+"\'");
    while(current_token < input.size()){
        ++current_token;
        if(current_token < input.size()){
            switch(input[current_token].get_type()){
            case game:
                current_token = result.input_game(input,current_token+1,msg);
                break;
            case board:
                current_token = result.input_board(input,current_token+1,msg);
                break;
            case players:
                current_token = result.input_players(input,current_token+1,msg);
                break;
            case variables:
                current_token = result.input_variables(input,current_token+1,msg);
                break;
            case pieces:
                current_token = result.input_pieces(input,current_token+1,msg);
                break;
            case rules:
                current_token = result.input_rules(input,current_token+1,msg);
                break;
            case identifier:
                current_token = result.input_macro(input,current_token,msg);
                break;
            default:
                throw msg.build_message(input[current_token].get_position(),"Expected \'game\', \'board\', \'players\', \'variables\', \'pieces\', \'rules\' token or identifier, encountered \'"+input[current_token].to_string()+"\'");
            }
        }
    }
    if(result.game_segment == nullptr)
        msg.add_message("No \'game\' directive");
    if(result.board_segment == nullptr)
        msg.add_message("No \'board\' directive");
    if(result.players_segment == nullptr)
        msg.add_message("No \'players\' directive");
    if(result.variables_segment == nullptr)
        msg.add_message("No \'variables\' directive");
    if(result.pieces_segment == nullptr)
        msg.add_message("No \'pieces\' directive");
    if(result.rules_segment == nullptr)
        msg.add_message("No \'rules\' directive");
    return result;
}

void game_items::print_slice(std::ostream& out,slice* segment,messages_container& msg)const throw(message){
    uint spaces_of_indent = 4;
    bool should_be_in_new_line = true;
    slice_iterator it(*segment,&macros);
    while(it.next(msg)){
        if(it.current(msg).get_type() == left_round_bracket || it.current(msg).get_type() == left_square_bracket){
            out<<'\n';
            print_spaces(out, spaces_of_indent);
            out<<it.current(msg).to_string();
            spaces_of_indent+=4;
            should_be_in_new_line = true;
        }
        else if(it.current(msg).get_type() == right_round_bracket || it.current(msg).get_type() == right_square_bracket){
            out<<'\n';
            if(spaces_of_indent>4)
                spaces_of_indent-=4;
            print_spaces(out, spaces_of_indent);
            out<<it.current(msg).to_string();
            should_be_in_new_line = true;
        }
        else if(it.current(msg).get_type() == plus){
            out<<'\n';
            print_spaces(out, spaces_of_indent-2);
            out<<it.current(msg).to_string()<<' ';
            should_be_in_new_line = false;
        }
        else{
            if(should_be_in_new_line){
                out<<'\n';
                print_spaces(out, spaces_of_indent);
                should_be_in_new_line = false;
            }
            out<<it.current(msg).to_string()<<' ';
        }
    }
    out<<'\n';
}

void game_items::print_segment(std::ostream& out,slice* game_items::*segment_position,const std::string& name,messages_container& msg)const throw(message){
    if(this->*segment_position){
        out<<"#"<<name<<" =";
        print_slice(out,this->*segment_position,msg);
    }
}

void game_items::print_rbg(std::ostream& out,messages_container& msg)const throw(message){
    print_segment(out,&game_items::game_segment,"game",msg);
    print_segment(out,&game_items::board_segment,"board",msg);
    print_segment(out,&game_items::players_segment,"players",msg);
    print_segment(out,&game_items::pieces_segment,"pieces",msg);
    print_segment(out,&game_items::variables_segment,"variables",msg);
    print_segment(out,&game_items::rules_segment,"rules",msg);
}

void print_spaces(std::ostream& out, uint n){
    for(uint i=0;i<n;++i)
        out<<' ';
}

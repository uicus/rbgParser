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
        msg.add_message(input[begin-2].get_position(),"Empty \'"+segment_name+"\' directive");
    if(this->*segment_position)
        msg.add_message(input[begin-2].get_position(),"Another \'"+segment_name+"\' declared; discarding previous");
    delete (this->*segment_position);
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
        if(input[current_token].get_type() != semicolon)
            throw msg.build_message(input[current_token].get_position(),"Expected \';\' or \'(\' token, encounered \'"+input[current_token].to_string()+"\'");
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
        throw msg.build_message("No \'game\' directive");
    if(result.board_segment == nullptr)
        throw msg.build_message("No \'board\' directive");
    if(result.players_segment == nullptr)
        throw msg.build_message("No \'players\' directive");
    if(result.variables_segment == nullptr)
        throw msg.build_message("No \'variables\' directive");
    if(result.pieces_segment == nullptr)
        throw msg.build_message("No \'pieces\' directive");
    if(result.rules_segment == nullptr)
        throw msg.build_message("No \'rules\' directive");
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

std::string game_items::parse_name(messages_container& msg)const throw(message){
    slice_iterator it(*game_segment,&macros);
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing \'game\' segment");
    it.next(msg);
    if(it.current(msg).get_type() != quotation)
        throw msg.build_message(it.create_call_stack("Expected quotes surrounded game name, encountered \'"+it.current(msg).to_string()+"\'"));
    std::string result = it.current(msg).to_string();
    if(it.next(msg))
        msg.add_message(it.create_call_stack("Unexpected tokens at the end of \'game\' segment"));
    return result;
}

std::set<token> game_items::parse_declaration_set(slice* game_items::*segment_position,const std::string& name,messages_container& msg)const throw(message){
    slice_iterator it(*(this->*segment_position),&macros);
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing \'"+name+"\' segment");
    it.next(msg);
    auto parsing_result = parse_sequence(it,name+" list",std::set<token>(),false,msg);
    if(!parsing_result.is_success())
        throw msg.build_message("Expected comma-separated list of identifiers in \'"+name+"\' segment");
    auto sequence = parsing_result.move_value();
    std::set<token> result(sequence.begin(),sequence.end());
    if(it.has_value())
        msg.add_message(it.create_call_stack("Unexpected tokens at the end of \'"+name+"\' segment"));
    return result;
}

void game_items::check_if_sets_disjoint(
const std::set<token>& s1,
const std::set<token>& s2,
const std::string sets_names,
messages_container& msg)const throw(message){
    for(const auto& el: s1)
        if(s2.find(el) != s2.end())
            throw msg.build_message("Identifier \'"+el.to_string()+"\' is found in both "+sets_names+" declarations");
}

declarations game_items::parse_declarations(messages_container& msg)const throw(message){
    declarations result(
        parse_declaration_set(&game_items::players_segment,"players",msg),
        parse_declaration_set(&game_items::pieces_segment,"pieces",msg),
        parse_declaration_set(&game_items::variables_segment,"variables",msg)
    );
    check_if_sets_disjoint(result.get_legal_pieces(),result.get_legal_players(),"\'pieces\' and \'players\'",msg);
    check_if_sets_disjoint(result.get_legal_pieces(),result.get_legal_variables(),"\'pieces\' and \'variables\'",msg);
    check_if_sets_disjoint(result.get_legal_players(),result.get_legal_variables(),"\'players\' and \'variables\'",msg);
    return result;
}

parser_result<std::vector<token>> game_items::parse_boardline(slice_iterator& it, const declarations& decl, messages_container& msg)const throw(message){
    if(!it.has_value() || it.current(msg).get_type() != left_square_bracket)
        return failure<std::vector<token>>();
    it.next(msg);
    auto result = parse_sequence(it,"board line",decl.get_legal_pieces(),true,msg);
    if(!result.is_success())
        throw msg.build_message(it.create_call_stack("Expected board line (comma separated pieces), encountered \'"+it.current(msg).to_string()+"\'"));
    if(it.current(msg).get_type() != right_square_bracket)
        throw msg.build_message(it.create_call_stack("Expected \']\', encountered \'"+it.current(msg).to_string()+"\'"));
    it.next(msg);
    return result;
}

game_board game_items::parse_board(const declarations& decl, messages_container& msg)const throw(message){
    slice_iterator it(*board_segment,&macros);
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing \'board\' segment");
    it.next(msg);
    game_board result;
    auto previous_position = it;
    auto next_line = parse_boardline(it,decl,msg);
    while(next_line.is_success()){
        if(!result.is_initialized() || result.get_width() == next_line.get_value().size())
            result.add_row(next_line.move_value());
        else
            throw msg.build_message(previous_position.create_call_stack("This line has "+std::to_string(next_line.get_value().size())+" cells while previous has "+std::to_string(result.get_width())));
        previous_position = it;
        next_line = parse_boardline(it,decl,msg);
    }
    if(it.has_value())
        msg.add_message(it.create_call_stack("Unexpected tokens at the end of \'board\' segment"));
    return result;
}

std::unique_ptr<game_move> game_items::parse_moves(const declarations& decl, messages_container& msg)const throw(message){
    slice_iterator it(*rules_segment,&macros);
    parsing_context_string_guard g(&it, "Unexpected end of input while parsing \'rules\' segment");
    it.next(msg);
    auto sum_result = parse_sum(it,decl,msg);
    if(!sum_result.is_success())
        throw msg.build_message("No rules given"); // We shouldn't reach this point anyway
    auto result = std::unique_ptr<game_move>(new sum(sum_result.move_value()));
    if(it.has_value())
        msg.add_message(it.create_call_stack("Unexpected tokens at the end of \'rules\' segment"));
    return result;
}

parsed_game game_items::parse_game(messages_container& msg)const throw(message){
    declarations decl = parse_declarations(msg);
    game_board brd = parse_board(decl,msg);
    std::unique_ptr<game_move> moves = parse_moves(decl,msg);
    return parsed_game(
        parse_name(msg),
        std::move(decl),
        std::move(brd),
        std::move(moves)
    );
}

void print_spaces(std::ostream& out, uint n){
    for(uint i=0;i<n;++i)
        out<<' ';
}

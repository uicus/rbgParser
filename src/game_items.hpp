#ifndef GAME_ITEMS
#define GAME_ITEMS

#include<map>
#include<set>
#include<vector>
#include<ostream>

#include"macro_bank.hpp"
#include"token.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"declarations.hpp"
#include"parsed_game.hpp"
#include"game_board.hpp"
#include"parser_helpers.hpp"
#include"sum.hpp"

namespace rbg_parser{

class unchecked_graph;
class graph;

class game_items{
        macro_bank macros;
        slice* game_segment;
        slice* board_segment;
        slice* players_segment;
        slice* variables_segment;
        slice* pieces_segment;
        slice* rules_segment;
        slice* bound_segment;
        uint next_item_context_order;
        game_items(void)noexcept;

        uint input_macro(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message);
        uint input_game(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message);
        uint input_board(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message);
        uint input_players(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message);
        uint input_variables(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message);
        uint input_pieces(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message);
        uint input_rules(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message);
        uint input_bound(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message);
        uint input_slice(
            const std::vector<token>& input,
            uint current_token,
            const std::string& segment_name,
            slice* game_items::*segment_position,
            bool should_be_nonempty,
            messages_container& msg)throw(message);
        void print_slice(std::ostream& out,slice* segment,messages_container& msg)const throw(message);
        void print_segment(std::ostream& out,slice* game_items::*segment_position,const std::string& name,messages_container& msg)const throw(message);
        std::string parse_name(messages_container& msg)const throw(message);
        std::set<token> parse_declaration_set(slice* game_items::*segment_position,const std::string& name,messages_container& msg)const throw(message);
        token parse_first_player(messages_container& msg)const throw(message);
        void check_if_sets_disjoint(
            const std::set<token>& s1,
            const std::set<token>& s2,
            const std::string sets_names,
            messages_container& msg)const throw(message);
        declarations parse_declarations(messages_container& msg)const throw(message);
        parser_result<std::vector<token>> parse_boardline(slice_iterator& it, const declarations& decl, messages_container& msg)const throw(message);
        game_board parse_board(const declarations& decl, messages_container& msg)const throw(message);
        unchecked_graph parse_unchecked_graph(declarations& decl, messages_container& msg)const throw(message);
        std::unique_ptr<game_move> parse_moves(const declarations& decl, slice* game_items::*segment_position, const std::string& name, messages_container& msg)const throw(message);
        uint parse_bound(messages_container& msg)const throw(message);
    public:
        game_items(const game_items&)=delete;
        game_items(game_items&& src)noexcept;
        game_items& operator=(const game_items&)=delete;
        game_items& operator=(game_items&& src)noexcept;
        ~game_items(void)noexcept;

        friend game_items input_tokens(const std::vector<token>& input,messages_container& msg)throw(message);
        void print_rbg(std::ostream& out,messages_container& msg)const throw(message);
        parsed_game parse_game(messages_container& msg)const throw(message);
};

uint reach_end_of_directive(const std::vector<token>& input,uint current_token);
uint parse_arguments(const std::vector<token>& input,uint current_token,std::vector<token>& args,messages_container& msg)throw(message);
game_items input_tokens(const std::vector<token>& input,messages_container& msg)throw(message);
void print_spaces(std::ostream& out, uint n);

}

#endif

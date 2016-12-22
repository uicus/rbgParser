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
#include"game_board.hpp"
#include"parsed_game.hpp"

class game_items{
        macro_bank macros;
        slice* game_segment;
        slice* board_segment;
        std::map<token,slice> player_segments;
        slice* order_segment;
        std::map<token,slice> goal_segments;
        uint next_item_context_order;
        game_items(void)noexcept;

        uint input_macro(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message);
        uint input_game(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message);
        uint input_board(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message);
        uint input_player(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message);
        uint input_order(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message);
        uint input_goal(const std::vector<token>& input,uint current_token,messages_container& msg)throw(message);

        void print_rbg_slice(const slice& s, std::ostream& out, messages_container& msg)const throw(message);
        void print_rbg_game(std::ostream& out, messages_container& msg)const throw(message);
        void print_rbg_board(std::ostream& out, messages_container& msg)const throw(message);
        void print_rbg_order(std::ostream& out, messages_container& msg)const throw(message);
        void print_rbg_players(std::ostream& out,messages_container& msg)const throw(message);
        void print_rbg_goals(std::ostream& out,messages_container& msg)const throw(message);

        game_board parse_board(messages_container& msg, std::set<token>& encountered_pieces)const throw(message);
        std::string parse_name(messages_container& msg)const throw(message);
    public:
        game_items(const game_items&)=delete;
        game_items(game_items&& src)noexcept;
        game_items& operator=(const game_items&)=delete;
        game_items& operator=(game_items&& src)noexcept;
        ~game_items(void)noexcept;

        friend game_items input_tokens(const std::vector<token>& input,messages_container& msg)throw(message);
        void print_rbg(std::ostream& out, messages_container& msg)const throw(message);
        parsed_game parse_game(messages_container& msg)const throw(message);
};

void print_tabs(std::ostream& out,uint n);
uint reach_end_of_directive(const std::vector<token>& input,uint current_token);
uint parse_arguments(const std::vector<token>& input,uint current_token,std::vector<token>& args,messages_container& msg)throw(message);
game_items input_tokens(const std::vector<token>& input,messages_container& msg)throw(message);

#endif

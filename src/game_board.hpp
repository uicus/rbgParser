#ifndef GAME_BOARD
#define GAME_BOARD

#include<vector>
#include<set>
#include<ostream>

#include"slice_iterator.hpp"
#include"message.hpp"
#include"types.hpp"
#include"token.hpp"

class game_board{
        uint width;
        uint height;
        uint currently_filled;
        std::vector<std::vector<token>> initial_position;
        bool is_completely_filled(void)const;
        void add_piece(const token& piece);
    public:
        game_board(uint width, uint height)noexcept;

        void fill_with_slice(slice_iterator& it,std::set<token>& encountered_pieces,messages_container& msg)throw(message);

        uint get_height(void)const;
        uint get_width(void)const;

        friend std::ostream& operator<<(std::ostream& out,const game_board& brd);
};

std::ostream& operator<<(std::ostream& out,const game_board& brd);

#endif

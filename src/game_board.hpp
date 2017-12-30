#ifndef GAME_BOARD
#define GAME_BOARD

#include<vector>

#include"token.hpp"

namespace rbg_parser{

class game_board{
        std::vector<std::vector<token>> squares;
    public:
        bool is_initialized(void)const;
        uint get_height(void)const;
        uint get_width(void)const;
        void add_row(std::vector<token>&& next_row);
        std::string to_rbg(bool pretty)const;
        const token& get_square(uint x,uint y)const;
};

}

#endif

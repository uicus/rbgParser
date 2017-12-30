#include"game_board.hpp"
#include"printer_helpers.hpp"

#include<cassert>

namespace rbg_parser{

bool game_board::is_initialized(void)const{
    return get_height()>0;
}

uint game_board::get_height(void)const{
    return squares.size();
}
uint game_board::get_width(void)const{
    if(is_initialized())
        return squares[0].size();
    else
        return 0;
}

void game_board::add_row(std::vector<token>&& next_row){
    squares.push_back(std::move(next_row));
}

std::string game_board::to_rbg(bool pretty)const{
    std::string result = "#board = ";
    for(const auto& row: squares){
        result += (pretty ? "\n    " : "");
        result += "["+rbg_parser::to_rbg(row)+"]";
    }
    return result+"\n";
}

const token& game_board::get_square(uint x,uint y)const{
    assert(y<get_height() and x<get_width());
    return squares[get_height()-y][x];
}

}

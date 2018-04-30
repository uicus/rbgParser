#include"shift.hpp"

namespace rbg_parser{

/*shift::shift(int x,int y):
x(x),
y(y){
}*/

shift::shift(const token& edge_name):
edge_name(edge_name){
}

void shift::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

std::string shift::to_rbg(uint)const{
    return to_rbg();
}

std::unique_ptr<game_move> shift::flatten(void){
    return std::unique_ptr<game_move>(new shift(std::move(*this)));
}

std::string shift::to_rbg()const{
    std::string result = "(";
    /*result += std::to_string(x);
    result += ",";
    result += std::to_string(y);*/
    result += edge_name.to_string();
    result += ")";
    return result;
}

const token& shift::get_content(void)const{
    return edge_name;
}

/*int shift::get_x(void)const{
    return x;
}

int shift::get_y(void)const{
    return y;
}*/

}

#ifndef PURE_GAME_MOVE
#define PURE_GAME_MOVE

#include"game_move.hpp"

namespace rbg_parser{

// interface
class pure_game_move : public game_move{
    public:
        ~pure_game_move(void)override=default;
        bool modifies(void)override{return false;}
        virtual std::unique_ptr<pure_game_move> pure_simplify(void)=0;
        std::unique_ptr<game_move> simplify(void)override{return pure_simplify();};
};

}

#endif

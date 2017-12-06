#ifndef PURE_GAME_MOVE
#define PURE_GAME_MOVE

#include"game_move.hpp"

// interface
class pure_game_move : public game_move{
    public:
        ~pure_game_move(void)override=default;
        bool modifies(void)override{return false;}
};

#endif

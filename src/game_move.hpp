#ifndef GAME_MOVE
#define GAME_MOVE

#include<memory>

class pure_game_move;
// interface
class game_move{
    public:
        virtual ~game_move(void)=default;
        virtual bool modifies(void){return true;}
        virtual std::unique_ptr<pure_game_move> transform_into_pure(void)=0;
};

#endif

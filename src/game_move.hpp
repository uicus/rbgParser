#ifndef GAME_MOVE
#define GAME_MOVE

class game_move{
    public:
        virtual ~game_move(void)=default;
        virtual bool modifies(void){return true;}
};

#endif

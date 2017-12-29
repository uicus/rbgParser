#ifndef GAME_MOVE
#define GAME_MOVE

#include<memory>

class pure_game_move;
class abstract_dispatcher;
// interface
class game_move{
    public:
        virtual ~game_move(void)=default;
        virtual bool modifies(void){return true;}
        virtual void set_lazy(void){};
        virtual std::unique_ptr<pure_game_move> transform_into_pure(void)=0;
        virtual void accept(abstract_dispatcher &dispatcher)const=0;
        virtual std::unique_ptr<game_move> simplify(void)=0;
        virtual uint priority(void)const=0; // being of higher priority containg lower ones requires surrounding them with brackets
        virtual std::string to_rbg(uint indent)const=0;
        virtual std::string to_rbg()const=0;
};

#endif

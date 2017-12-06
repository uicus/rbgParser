#ifndef PURE_BRACKETED_MOVE
#define PURE_BRACKETED_MOVE

#include<memory>

#include"pure_game_move.hpp"
#include"types.hpp"

class bracketed_move;

class pure_bracketed_move : public pure_game_move{
        std::unique_ptr<pure_game_move> contained_move;
        uint number_of_repetitions;
        pure_bracketed_move(void);
        pure_bracketed_move(std::unique_ptr<pure_game_move> contained_move, uint number_of_repetitions=1);
    public:
        ~pure_bracketed_move(void)override=default;
        pure_bracketed_move(pure_bracketed_move&&)=default;
        pure_bracketed_move(const pure_bracketed_move&)=default;
        pure_bracketed_move& operator=(const pure_bracketed_move&)=default;
        pure_bracketed_move& operator=(pure_bracketed_move&&)=default;
        std::unique_ptr<pure_game_move> transform_into_pure(void)override;
        friend class bracketed_move;
};

#endif

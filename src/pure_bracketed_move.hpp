#ifndef PURE_BRACKETED_MOVE
#define PURE_BRACKETED_MOVE

#include<memory>

#include"pure_game_move.hpp"
#include"types.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class bracketed_move;

class pure_bracketed_move : public pure_game_move{
        std::unique_ptr<pure_game_move> contained_move;
        uint number_of_repetitions;
        pure_bracketed_move(void);
        pure_bracketed_move(std::unique_ptr<pure_game_move> contained_move, uint number_of_repetitions=1);
        std::string print_power(void)const;
    public:
        ~pure_bracketed_move(void)override=default;
        pure_bracketed_move(pure_bracketed_move&&)=default;
        pure_bracketed_move(const pure_bracketed_move&)=default;
        pure_bracketed_move& operator=(const pure_bracketed_move&)=default;
        pure_bracketed_move& operator=(pure_bracketed_move&&)=default;
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(void)const override{return 2;};
        std::string to_rbg(uint indent)const override;
        std::string to_rbg()const override;
        const pure_game_move* get_content(void)const;
        uint get_number_of_repetitions(void)const;
        bool is_star(void)const;
        std::unique_ptr<pure_game_move> transform_into_pure(void)override;
        std::unique_ptr<pure_game_move> pure_simplify(void)override;
        friend class bracketed_move;
};

}

#endif

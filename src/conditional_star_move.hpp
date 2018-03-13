#ifndef CONDITIONAL_STAR_MOVE
#define CONDITIONAL_STAR_MOVE

#include<memory>

#include"game_move.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class conditional_star_move : public game_move{
        std::unique_ptr<game_move> contained_move;
        conditional_star_move(void)=default;
    public:
        conditional_star_move(std::unique_ptr<game_move> contained_move);
        ~conditional_star_move(void)override=default;
        conditional_star_move(conditional_star_move&&)=default;
        conditional_star_move(const conditional_star_move&)=default;
        conditional_star_move& operator=(const conditional_star_move&)=default;
        conditional_star_move& operator=(conditional_star_move&&)=default;
        bool modifies(void)const override;
        void set_lazy(void)override{contained_move->set_lazy();};
        std::unique_ptr<game_move> simplify(void)override;
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(void)const override{return 3;};
        std::string to_rbg(uint indent)const override;
        std::string to_rbg()const override;
        std::unique_ptr<game_move> flatten(void)override;
        straightness_result compute_k_straightness(void)const override;
        const game_move* get_content(void)const;
};

}

#endif

#ifndef CONDITIONAL_SUM
#define CONDITIONAL_SUM

#include<vector>
#include<memory>

#include"game_move.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class conditional_sum : public game_move{
        std::vector<std::unique_ptr<game_move>> content;
    public:
        conditional_sum(std::vector<std::unique_ptr<game_move>>&& content);
        ~conditional_sum(void)override=default;
        conditional_sum(void)=default;
        conditional_sum(conditional_sum&&)=default;
        conditional_sum(const conditional_sum&)=default;
        conditional_sum& operator=(const conditional_sum&)=default;
        conditional_sum& operator=(conditional_sum&&)=default;
        bool modifies(void)const override;
        void set_lazy(void)override;
        std::unique_ptr<game_move> simplify(void)override;
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(void)const override{return 0;};
        std::string to_rbg(uint indent)const override;
        std::string to_rbg()const override;
        std::unique_ptr<game_move> flatten(void)override;
        straightness_result compute_k_straightness(void)const override;
        const std::vector<std::unique_ptr<game_move>>& get_content(void)const;
};

}

#endif

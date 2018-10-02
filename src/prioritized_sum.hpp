#ifndef PRIORITIZED_SUM
#define PRIORITIZED_SUM

#include<vector>
#include<memory>

#include"game_move.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class prioritized_sum : public game_move{
        std::vector<std::unique_ptr<game_move>> content;
    public:
        prioritized_sum(std::vector<std::unique_ptr<game_move>>&& content);
        ~prioritized_sum(void)override=default;
        prioritized_sum(void)=default;
        prioritized_sum(prioritized_sum&&)=default;
        prioritized_sum(const prioritized_sum&)=delete;
        prioritized_sum& operator=(const prioritized_sum&)=delete;
        prioritized_sum& operator=(prioritized_sum&&)=default;
        bool modifies(void)const override;
        std::unique_ptr<game_move> simplify(void)override;
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(void)const override{return 1;};
        std::string to_rbg(uint indent)const override;
        std::string to_rbg()const override;
        std::unique_ptr<game_move> flatten(void)override;
        straightness_result compute_k_straightness(void)const override;
        const std::vector<std::unique_ptr<game_move>>& get_content(void)const;
        std::unique_ptr<game_move> copy(void)const override;
        bool has_piece_as_variable(const std::set<token>& pieces)const override;
};

}

#endif

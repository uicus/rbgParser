#ifndef SUM
#define SUM

#include<vector>
#include<memory>

#include"game_move.hpp"
#include"abstract_dispatcher.hpp"
#include"options.hpp"

namespace rbg_parser{

class sum : public game_move{
        std::vector<std::unique_ptr<game_move>> content;
    public:
        sum(std::vector<std::unique_ptr<game_move>>&& content);
        ~sum(void)override=default;
        sum(void)=default;
        sum(sum&&)=default;
        sum(const sum&)=delete;
        sum& operator=(const sum&)=delete;
        sum& operator=(sum&&)=default;
        int give_indices_in_expression(int next_free)override;
        bool modifies(void)const override;
        std::unique_ptr<game_move> simplify(void)override;
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(const options& opt)const override{return opt.enabled_noop_before_alternative() ? 2 : 1;};
        std::string to_rbg(const options& opts, uint indent)const override;
        std::string to_rbg(const options& opts)const override;
        std::unique_ptr<game_move> flatten(void)override;
        void gather_sum_elements(std::vector<std::unique_ptr<game_move>>& elements)override;
        straightness_result compute_k_straightness(void)const override;
        const std::vector<std::unique_ptr<game_move>>& get_content(void)const;
        void add_move(std::unique_ptr<game_move> m);
        std::unique_ptr<game_move> copy(void)const override;
        bool has_piece_as_variable(const std::set<token>& pieces)const override;
};

}

#endif

#ifndef CONCATENATION
#define CONCATENATION

#include<vector>
#include<memory>

#include"game_move.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class concatenation : public game_move{
    protected:
        std::vector<std::unique_ptr<game_move>> content;
    public:
        concatenation(std::vector<std::unique_ptr<game_move>>&& content);
        ~concatenation(void)override=default;
        concatenation(void)=default;
        concatenation(concatenation&&)=default;
        concatenation(const concatenation&)=delete;
        concatenation& operator=(const concatenation&)=delete;
        concatenation& operator=(concatenation&&)=default;
        int give_indices_in_expression(int next_free)override;
        bool modifies(void)const override;
        std::unique_ptr<game_move> simplify(void)override;
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(const options&)const override{return 2;};
        std::string to_rbg(const options& opt, uint indent)const override;
        std::string to_rbg(const options& opt)const override;
        std::unique_ptr<game_move> flatten(void)override;
        void gather_concatenation_elements(std::vector<std::unique_ptr<game_move>>& elements)override;
        straightness_result compute_k_straightness(StraightnessType st)const override;
        const std::vector<std::unique_ptr<game_move>>& get_content(void)const;
        void add_move(std::unique_ptr<game_move> m);
        std::unique_ptr<game_move> copy(void)const override;
        bool has_piece_as_variable(const std::set<token>& pieces)const override;
};

}

#endif

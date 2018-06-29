#ifndef MOVE_CHECK
#define MOVE_CHECK

#include<memory>

#include"game_move.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class move_check : public game_move{
        std::unique_ptr<game_move> content;
        bool negated;
        move_check(void)=default;
    public:
        move_check(std::unique_ptr<game_move> content, bool negated);
        ~move_check(void)override=default;
        move_check(move_check&&)=default;
        move_check(const move_check&)=default;
        move_check& operator=(const move_check&)=default;
        move_check& operator=(move_check&&)=default;
        void accept(abstract_dispatcher& dispatcher)const override;
        const game_move* get_content(void)const;
        bool is_negated(void)const;
        std::unique_ptr<game_move> simplify(void)override;
        uint priority(void)const override{return 4;};
        straightness_result compute_k_straightness(void)const override;
        std::string to_rbg(uint indent)const override;
        std::string to_rbg()const override;
        void gather_concatenation_elements(
            std::vector<std::unique_ptr<game_move>>& elements,
            std::vector<std::unique_ptr<game_move>>& next_block_elements)override;
        std::unique_ptr<game_move> flatten(void)override;
};

}

#endif

#ifndef POWER_MOVE
#define POWER_MOVE

#include<memory>

#include"game_move.hpp"
#include"types.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class power_move : public game_move{
        std::unique_ptr<game_move> contained_move;
        uint number_of_repetitions;
        power_move(void);
        std::string print_power(void)const;
    public:
        power_move(std::unique_ptr<game_move> contained_move, uint number_of_repetitions=1);
        ~power_move(void)override=default;
        power_move(power_move&&)=default;
        power_move(const power_move&)=default;
        power_move& operator=(const power_move&)=default;
        power_move& operator=(power_move&&)=default;
        bool modifies(void)const override;
        std::unique_ptr<game_move> simplify(void)override;
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(void)const override{return 3;};
        std::string to_rbg(uint indent)const override;
        std::string to_rbg()const override;
        std::unique_ptr<game_move> flatten(void)override;
        straightness_result compute_k_straightness(void)const override;
        const game_move* get_content(void)const;
        uint get_number_of_repetitions(void)const;
};

}

#endif

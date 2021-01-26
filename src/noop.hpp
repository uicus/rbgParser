#ifndef NOOP
#define NOOP

#include<set>
#include<vector>

#include"game_move.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class noop : public game_move{
    public:
        noop(void)=default;
        ~noop(void)override=default;
        noop(noop&&)=default;
        noop(const noop&)=delete;
        noop& operator=(const noop&)=delete;
        noop& operator=(noop&&)=default;
        int give_indices_in_expression(int next_free)override;
        std::unique_ptr<game_move> simplify(void)override{return std::unique_ptr<game_move>(new noop);};
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(const options&)const override{return 4;};
        straightness_result compute_k_straightness(StraightnessType st)const override;
        std::string to_rbg(const options& opt, uint)const override;
        std::string to_rbg(const options&)const override;
        std::unique_ptr<game_move> flatten(void)override;
        std::unique_ptr<game_move> copy(void)const override;
};

}

#endif

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
        uint priority(void)const override{return 4;};
        std::string to_rbg(uint)const override;
        std::string to_rbg()const override;
        std::unique_ptr<game_move> flatten(void)override;
        std::unique_ptr<game_move> copy(void)const override;
};

}

#endif

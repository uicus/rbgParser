#ifndef SLOTH
#define SLOTH

#include"game_move.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class sloth : public game_move{
    public:
        sloth(void)=default;
        ~sloth(void)override=default;
        sloth(sloth&&)=default;
        sloth(const sloth&)=default;
        sloth& operator=(const sloth&)=default;
        sloth& operator=(sloth&&)=default;
        std::unique_ptr<game_move> simplify(void)override{return std::unique_ptr<game_move>(new sloth(std::move(*this)));};
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(void)const override{return 4;};
        std::string to_rbg(uint)const override;
        std::string to_rbg()const override;
        std::unique_ptr<game_move> flatten(void)override;
};

}

#endif

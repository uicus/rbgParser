#ifndef ACTIONS_BLOCK
#define ACTIONS_BLOCK

#include"concatenation.hpp"

namespace rbg_parser{

class actions_block : public concatenation{
    public:
        actions_block(std::vector<std::unique_ptr<game_move>>&& content);
        void accept(abstract_dispatcher& dispatcher)const override;
        bool modifies(void)const override{return true;};
        bool has_finisher(void)const override{return content.back()->has_finisher();};
        std::unique_ptr<game_move> copy(void)const override;
};

}

#endif

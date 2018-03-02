#ifndef PLAYER_CHECK
#define PLAYER_CHECK

#include"condition.hpp"
#include"token.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class player_check : public condition{
        token player_to_check;
        bool equal_check; // if false then neq
        player_check(void)=default;
    public:
        player_check(token player_to_check, bool equal_check);
        ~player_check(void)override=default;
        player_check(player_check&&)=default;
        player_check(const player_check&)=default;
        player_check& operator=(const player_check&)=default;
        player_check& operator=(player_check&&)=default;
        void accept(abstract_dispatcher& dispatcher)const override;
        const token& get_player_to_check(void)const;
        bool should_be_equal(void)const;
        std::unique_ptr<condition> condition_simplify(void)override;
        uint priority(void)const override{return 3;};
        std::string to_rbg(uint)const override;
        std::string to_rbg()const override;
        std::unique_ptr<condition> condition_flatten(void)override;
};

}

#endif

#ifndef CONDITION_CHECK
#define CONDITION_CHECK

#include<memory>

#include"condition.hpp"
#include"game_move.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class condition_check : public game_move{
        std::unique_ptr<condition> content;
        bool negated;
        condition_check(void)=default;
    public:
        condition_check(std::unique_ptr<condition> content, bool negated);
        ~condition_check(void)override=default;
        condition_check(condition_check&&)=default;
        condition_check(const condition_check&)=default;
        condition_check& operator=(const condition_check&)=default;
        condition_check& operator=(condition_check&&)=default;
        void accept(abstract_dispatcher& dispatcher)const override;
        const condition* get_content(void)const;
        bool is_negated(void)const;
        std::unique_ptr<game_move> simplify(void)override;
        uint priority(void)const override{return 4;};
        std::string to_rbg(uint indent)const override;
        std::string to_rbg()const override;
        std::unique_ptr<game_move> flatten(void)override;
};

}

#endif

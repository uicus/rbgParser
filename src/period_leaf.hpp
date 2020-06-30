#ifndef PERIOD_LEAF
#define PERIOD_LEAF

#include"tree_utils.hpp"
#include"parser_helpers.hpp"
#include<memory>

namespace rbg_parser{

class slice_iterator;
class messages_container;
class message;

class period_leaf : public expression{
        period_leaf(slice_iterator&& beginning_position);
    public:
        period_leaf(void)=delete;
        period_leaf(const period_leaf&)=default;
        period_leaf(period_leaf&&)=default;
        period_leaf& operator=(period_leaf&&)=default;
        period_leaf& operator=(const period_leaf&)=default;
        virtual ~period_leaf(void)=default;
        expression_type get_type(void)const override;
        void type(const typing_machine& m, messages_container& msg)override;
        std::unique_ptr<game_move> get_game_move(void)const override;
        friend parser_result<std::unique_ptr<expression>> parse_period_leaf(slice_iterator& it, messages_container& msg);
};

parser_result<std::unique_ptr<expression>> parse_period_leaf(slice_iterator& it, messages_container& msg);

}

#endif

#ifndef SLOTH_LEAF
#define SLOTH_LEAF

#include"tree_utils.hpp"
#include"parser_helpers.hpp"

namespace rbg_parser{

class slice_iterator;
class messages_container;
class message;
class typing_machine;

class sloth_leaf : public expression{
        sloth_leaf(slice_iterator&& beginning_position);
    public:
        sloth_leaf(void)=delete;
        sloth_leaf(const sloth_leaf&)=default;
        sloth_leaf(sloth_leaf&&)=default;
        sloth_leaf& operator=(sloth_leaf&&)=default;
        sloth_leaf& operator=(const sloth_leaf&)=default;
        virtual ~sloth_leaf(void)=default;
        expression_type get_type(void)const override;
        void type(const typing_machine&, messages_container&)throw(message)override;
        virtual std::unique_ptr<game_move> get_game_move(void)const;
        friend parser_result<std::unique_ptr<expression>> parse_sloth_leaf(slice_iterator& it, messages_container& msg)throw(message);
};

parser_result<std::unique_ptr<expression>> parse_sloth_leaf(slice_iterator& it, messages_container& msg)throw(message);

}

#endif

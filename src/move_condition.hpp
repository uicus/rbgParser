#ifndef MOVE_CONDITION
#define MOVE_CONDITION

#include<memory>

#include"condition.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"
#include"pure_game_move.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class move_condition : public condition{
        std::unique_ptr<pure_game_move> content;
        move_condition(void)=default;
        move_condition(std::unique_ptr<pure_game_move> content);
    public:
        ~move_condition(void)override=default;
        move_condition(move_condition&&)=default;
        move_condition(const move_condition&)=default;
        move_condition& operator=(const move_condition&)=default;
        move_condition& operator=(move_condition&&)=default;
        void accept(abstract_dispatcher& dispatcher)const override;
        const pure_game_move* get_content(void)const;
        std::unique_ptr<condition> simplify(void)override;
        uint priority(void)const override{return 3;};
        std::string to_rbg(uint indent)const override;
        std::string to_rbg()const override;
        std::unique_ptr<condition> flatten(void)override;
        friend parser_result<move_condition> parse_move_condition(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<move_condition>;
};

parser_result<move_condition> parse_move_condition(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

}

#endif

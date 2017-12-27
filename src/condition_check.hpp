#ifndef CONDITION_CHECK
#define CONDITION_CHECK

#include<memory>

#include"condition.hpp"
#include"pure_game_move.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"
#include"abstract_dispatcher.hpp"

class condition_check : public pure_game_move{
        std::unique_ptr<condition> content;
        condition_check(void)=default;
        condition_check(std::unique_ptr<condition> content);
    public:
        ~condition_check(void)override=default;
        condition_check(condition_check&&)=default;
        condition_check(const condition_check&)=default;
        condition_check& operator=(const condition_check&)=default;
        condition_check& operator=(condition_check&&)=default;
        void accept(abstract_dispatcher& dispatcher)const override;
        const condition* get_content(void)const;
        std::unique_ptr<pure_game_move> transform_into_pure(void)override{return std::unique_ptr<pure_game_move>(new condition_check(std::move(content)));};
        std::unique_ptr<pure_game_move> pure_simplify(void)override;
        friend parser_result<condition_check> parse_condition_check(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<condition_check>;
};

parser_result<condition_check> parse_condition_check(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

#endif

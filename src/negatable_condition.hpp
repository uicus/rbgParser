#ifndef NEGATABLE_CONDITION
#define NEGATABLE_CONDITION

#include<memory>

#include"condition.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"
#include"abstract_dispatcher.hpp"

class negatable_condition : public condition{
        std::unique_ptr<condition> content;
        bool modifier;
        negatable_condition(void)=default;
        negatable_condition(std::unique_ptr<condition> content, bool modifier);
    public:
        ~negatable_condition(void)override=default;
        negatable_condition(negatable_condition&&)=default;
        negatable_condition(const negatable_condition&)=default;
        negatable_condition& operator=(const negatable_condition&)=default;
        negatable_condition& operator=(negatable_condition&&)=default;
        void accept(abstract_dispatcher& dispatcher)override;
        const condition* get_content(void)const;
        bool is_negated(void)const;
        std::unique_ptr<condition> simplify(void)override;
        friend parser_result<negatable_condition> parse_negatable_condition(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<negatable_condition>;
};

parser_result<negatable_condition> parse_negatable_condition(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

#endif

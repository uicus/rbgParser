#ifndef NEGATABLE_CONDITION
#define NEGATABLE_CONDITION

#include<memory>

#include"condition.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"

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
        friend parser_result<negatable_condition> parse_negatable_condition(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<negatable_condition>;
};

parser_result<negatable_condition> parse_negatable_condition(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

#endif

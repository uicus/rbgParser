#ifndef VARIABLE_LEAF
#define VARIABLE_LEAF

#include"tree_utils.hpp"
#include"parser_helpers.hpp"
#include<memory>

namespace rbg_parser{

class slice_iterator;
class messages_container;
class message;

class variable_leaf : public expression{
        token name;
        expression_type identifier_type;
        variable_leaf(slice_iterator&& beginning_position, const token& name);
    public:
        variable_leaf(void)=delete;
        variable_leaf(const variable_leaf&)=default;
        variable_leaf(variable_leaf&&)=default;
        variable_leaf& operator=(variable_leaf&&)=default;
        variable_leaf& operator=(const variable_leaf&)=default;
        virtual ~variable_leaf(void)=default;
        expression_type get_type(void)const override;
        void type(const typing_machine& m, messages_container& msg)throw(message)override;
        token get_identifier(void)const override;
        std::unique_ptr<arithmetic_expression> get_arithmetic_expression(void)const override;
        friend parser_result<std::unique_ptr<expression>> parse_variable_leaf(slice_iterator& it, messages_container& msg)throw(message);
};

parser_result<std::unique_ptr<expression>> parse_variable_leaf(slice_iterator& it, messages_container& msg)throw(message);

}

#endif

#ifndef MULTIPLY_ARITHMETIC
#define MULTIPLY_ARITHMETIC

#include"arithmetic_expression.hpp"
#include"token.hpp"
#include<vector>
#include<memory>

namespace rbg_parser{

class multiply_arithmetic : public arithmetic_expression{
        std::vector<std::unique_ptr<arithmetic_expression>> content;
    public:
        multiply_arithmetic(std::vector<std::unique_ptr<arithmetic_expression>>&& content);
        multiply_arithmetic(const multiply_arithmetic&)=default;
        multiply_arithmetic(multiply_arithmetic&&)=default;
        multiply_arithmetic& operator=(const multiply_arithmetic&)=default;
        multiply_arithmetic& operator=(multiply_arithmetic&&)=default;
        ~multiply_arithmetic(void)override=default;
        void accept(abstract_dispatcher &dispatcher)const override;
        std::unique_ptr<arithmetic_expression> simplify(void)override;
        uint priority(void)const override{return 2;};
        std::string to_rbg(uint)const override;
        std::string to_rbg()const override;
        std::unique_ptr<arithmetic_expression> flatten(void)override;
        void gather_multiply_elements(std::vector<std::unique_ptr<arithmetic_expression>>& elements)override;
        const std::vector<std::unique_ptr<arithmetic_expression>>& get_content(void)const;
};

}

#endif

#ifndef SUM_ARITHMETIC
#define SUM_ARITHMETIC

#include"arithmetic_expression.hpp"
#include"token.hpp"
#include<vector>
#include<memory>

namespace rbg_parser{

class sum_arithmetic : public arithmetic_expression{
        std::vector<std::unique_ptr<arithmetic_expression>> content;
    public:
        sum_arithmetic(std::vector<std::unique_ptr<arithmetic_expression>>&& content);
        sum_arithmetic(const sum_arithmetic&)=default;
        sum_arithmetic(sum_arithmetic&&)=default;
        sum_arithmetic& operator=(const sum_arithmetic&)=default;
        sum_arithmetic& operator=(sum_arithmetic&&)=default;
        ~sum_arithmetic(void)override=default;
        void accept(abstract_dispatcher &dispatcher)const override;
        std::unique_ptr<arithmetic_expression> simplify(void)override;
        uint priority(void)const override{return 1;};
        std::string to_rbg(uint)const override;
        std::string to_rbg()const override;
        std::unique_ptr<arithmetic_expression> flatten(void)override;
        void gather_sum_elements(std::vector<std::unique_ptr<arithmetic_expression>>& elements)override;
        const std::vector<std::unique_ptr<arithmetic_expression>>& get_content(void)const;
};

}

#endif

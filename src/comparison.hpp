#ifndef COMPARISON
#define COMPARISON

#include"condition.hpp"
#include"token.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class comparison : public condition{
        token left_side;
        token kind_of_comparison;
        token right_side;
        comparison(void)=default;
        comparison(token left_side,token kind_of_comparison,token right_side);
        void sort(void);
        void reverse(void);
    public:
        ~comparison(void)override=default;
        comparison(comparison&&)=default;
        comparison(const comparison&)=default;
        comparison& operator=(const comparison&)=default;
        comparison& operator=(comparison&&)=default;
        void accept(abstract_dispatcher& dispatcher)const override;
        const token& get_left_side(void)const;
        const token& get_kind_of_comparison(void)const;
        const token& get_right_side(void)const;
        std::unique_ptr<condition> simplify(void)override{return std::unique_ptr<condition>(new comparison(std::move(*this)));};
        uint priority(void)const override{return 3;};
        std::string to_rbg(uint)const override;
        std::string to_rbg()const override;
        friend parser_result<comparison> parse_comparison(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<comparison>;
};

parser_result<comparison> parse_comparison(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

}

#endif

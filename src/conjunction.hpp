#ifndef CONJUNCTION
#define CONJUNCTION

#include<vector>
#include<memory>

#include"condition.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class conjunction : public condition{
        std::vector<std::unique_ptr<condition>> content;
        conjunction(void)=default;
        conjunction(std::vector<std::unique_ptr<condition>> content);
    public:
        ~conjunction(void)override=default;
        conjunction(conjunction&&)=default;
        conjunction(const conjunction&)=default;
        conjunction& operator=(const conjunction&)=default;
        conjunction& operator=(conjunction&&)=default;
        void accept(abstract_dispatcher& dispatcher)const override;
        const std::vector<std::unique_ptr<condition>>& get_content(void)const;
        std::unique_ptr<condition> simplify(void)override;
        uint priority(void)const override{return 1;};
        std::string to_rbg(uint indent)const override;
        std::string to_rbg()const override;
        std::unique_ptr<condition> flatten(void)override;
        void gather_conjunction_elements(std::vector<std::unique_ptr<condition>>& elements)override;
        friend parser_result<conjunction> parse_conjunction(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<conjunction>;
};

parser_result<conjunction> parse_conjunction(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

}

#endif

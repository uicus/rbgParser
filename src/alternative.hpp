#ifndef ALTERNATIVE
#define ALTERNATIVE

#include<vector>
#include<memory>

#include"condition.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"
#include"abstract_dispatcher.hpp"

class alternative : public condition{
        std::vector<std::unique_ptr<condition>> content;
        alternative(void)=default;
        alternative(std::vector<std::unique_ptr<condition>> content);
    public:
        ~alternative(void)override=default;
        alternative(alternative&&)=default;
        alternative(const alternative&)=default;
        alternative& operator=(const alternative&)=default;
        alternative& operator=(alternative&&)=default;
        void accept(abstract_dispatcher& dispatcher)const override;
        const std::vector<std::unique_ptr<condition>>& get_content(void)const;
        std::unique_ptr<condition> simplify(void)override;
        uint priority(void)const override{return 0;};
        std::string to_rbg(uint indent)const override;
        std::string to_rbg()const override;
        friend parser_result<alternative> parse_alternative(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<alternative>;
};

parser_result<alternative> parse_alternative(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

#endif

#ifndef ALTERNATIVE
#define ALTERNATIVE

#include<vector>
#include<memory>

#include"condition.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"

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
        friend parser_result<alternative> parse_alternative(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<alternative>;
};

parser_result<alternative> parse_alternative(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

#endif

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
        void accept(abstract_dispatcher& dispatcher)override;
        const std::vector<std::unique_ptr<condition>>& get_content(void)const;
        std::unique_ptr<condition> simplify(void)override;
        friend parser_result<conjunction> parse_conjunction(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<conjunction>;
};

parser_result<conjunction> parse_conjunction(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

#endif

#ifndef CONCATENATION
#define CONCATENATION

#include<vector>
#include<memory>

#include"game_move.hpp"
#include"types.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"

class concatenation : public game_move{
        std::vector<std::unique_ptr<game_move>> content;
        concatenation(void)=default;
        concatenation(std::vector<std::unique_ptr<game_move>>&& content);
    public:
        ~concatenation(void)override=default;
        concatenation(concatenation&&)=default;
        concatenation(const concatenation&)=default;
        concatenation& operator=(const concatenation&)=default;
        concatenation& operator=(concatenation&&)=default;
        bool modifies(void)override;
        friend parser_result<concatenation> parse_concatenation(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<concatenation>;
};

parser_result<concatenation> parse_concatenation(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

#endif

#ifndef SUM
#define SUM

#include<vector>
#include<memory>

#include"game_move.hpp"
#include"types.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"

class sum : public game_move{
        std::vector<std::unique_ptr<game_move>> content;
        sum(void)=default;
        sum(std::vector<std::unique_ptr<game_move>>&& content);
    public:
        ~sum(void)override=default;
        sum(sum&&)=default;
        sum(const sum&)=default;
        sum& operator=(const sum&)=default;
        sum& operator=(sum&&)=default;
        bool modifies(void)override;
        friend parser_result<sum> parse_sum(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<sum>;
};

parser_result<sum> parse_sum(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

#endif

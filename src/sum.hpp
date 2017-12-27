#ifndef SUM
#define SUM

#include<vector>
#include<memory>

#include"game_move.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"
#include"abstract_dispatcher.hpp"

class sum : public game_move{
        std::vector<std::unique_ptr<game_move>> content;
        sum(std::vector<std::unique_ptr<game_move>>&& content);
    public:
        ~sum(void)override=default;
        sum(void)=default;
        sum(sum&&)=default;
        sum(const sum&)=default;
        sum& operator=(const sum&)=default;
        sum& operator=(sum&&)=default;
        bool modifies(void)override;
        void set_lazy(void)override;
        std::unique_ptr<pure_game_move> transform_into_pure(void)override;
        std::unique_ptr<game_move> simplify(void)override;
        void accept(abstract_dispatcher& dispatcher)const override;
        const std::vector<std::unique_ptr<game_move>>& get_content(void)const;
        void add_move(std::unique_ptr<game_move> m);
        friend parser_result<sum> parse_sum(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<sum>;
};

parser_result<sum> parse_sum(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

#endif

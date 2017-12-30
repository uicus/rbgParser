#ifndef CONCATENATION
#define CONCATENATION

#include<vector>
#include<memory>

#include"game_move.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class concatenation : public game_move{
        std::vector<std::unique_ptr<game_move>> content;
        concatenation(std::vector<std::unique_ptr<game_move>>&& content);
    public:
        ~concatenation(void)override=default;
        concatenation(void)=default;
        concatenation(concatenation&&)=default;
        concatenation(const concatenation&)=default;
        concatenation& operator=(const concatenation&)=default;
        concatenation& operator=(concatenation&&)=default;
        bool modifies(void)override;
        void set_lazy(void)override;
        std::unique_ptr<pure_game_move> transform_into_pure(void)override;
        std::unique_ptr<game_move> simplify(void)override;
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(void)const override{return 1;};
        std::string to_rbg(uint indent)const override;
        std::string to_rbg()const override;
        const std::vector<std::unique_ptr<game_move>>& get_content(void)const;
        void add_move(std::unique_ptr<game_move> m);
        friend parser_result<concatenation> parse_concatenation(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<concatenation>;
};

parser_result<concatenation> parse_concatenation(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

}

#endif

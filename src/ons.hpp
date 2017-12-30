#ifndef ONS
#define ONS

#include<set>
#include<vector>

#include"pure_game_move.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class ons : public pure_game_move{
        std::set<token> legal_ons;
        ons(std::vector<token>&& legal_ons);
        ons(void)=default;
    public:
        ~ons(void)override=default;
        ons(ons&&)=default;
        ons(const ons&)=default;
        ons& operator=(const ons&)=default;
        ons& operator=(ons&&)=default;
        std::unique_ptr<pure_game_move> transform_into_pure(void)override{return std::unique_ptr<pure_game_move>(new ons(std::move(*this)));};
        std::unique_ptr<pure_game_move> pure_simplify(void)override{return std::unique_ptr<pure_game_move>(new ons(std::move(*this)));};
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(void)const override{return 3;};
        std::string to_rbg(uint)const override;
        std::string to_rbg()const override;
        const std::set<token>& get_legal_ons(void)const;
        friend parser_result<ons> parse_ons(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<ons>;
};

parser_result<ons> parse_ons(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

}

#endif

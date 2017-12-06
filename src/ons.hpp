#ifndef ONS
#define ONS

#include<set>
#include<vector>

#include"pure_game_move.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"

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
        friend parser_result<ons> parse_ons(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<ons>;
};

parser_result<ons> parse_ons(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

#endif

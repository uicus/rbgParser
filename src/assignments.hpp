#ifndef ASSIGNMENTS
#define ASSIGNMENTS

#include<cassert>

#include"game_move.hpp"
#include"concatenation.hpp"
#include"token.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"

class assignment : public game_move{
        token left_side;
        token right_side;
        assignment(token&& left_side, token&& right_side);
        assignment(void)=default;
    public:
        ~assignment(void)override=default;
        assignment(assignment&&)=default;
        assignment(const assignment&)=default;
        assignment& operator=(const assignment&)=default;
        assignment& operator=(assignment&&)=default;
        std::unique_ptr<pure_game_move> transform_into_pure(void)override{assert(false);};
        friend parser_result<assignment> parse_single_assignment(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<assignment>;
        friend parser_result<concatenation> parse_assignments(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
};

parser_result<concatenation> parse_assignments(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

#endif

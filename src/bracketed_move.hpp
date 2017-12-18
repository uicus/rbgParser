#ifndef BRACKETED_MOVE
#define BRACKETED_MOVE

#include<memory>

#include"game_move.hpp"
#include"types.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"declarations.hpp"
#include"abstract_dispatcher.hpp"

class bracketed_move : public game_move{
        std::unique_ptr<game_move> contained_move;
        uint number_of_repetitions;
        bracketed_move(void);
        bracketed_move(std::unique_ptr<game_move> contained_move, uint number_of_repetitions=1);
    public:
        ~bracketed_move(void)override=default;
        bracketed_move(bracketed_move&&)=default;
        bracketed_move(const bracketed_move&)=default;
        bracketed_move& operator=(const bracketed_move&)=default;
        bracketed_move& operator=(bracketed_move&&)=default;
        bool modifies(void)override;
        void set_lazy(void)override{contained_move->set_lazy();};
        std::unique_ptr<pure_game_move> transform_into_pure(void)override;
        void accept(abstract_dispatcher& dispatcher)override;
        const game_move* get_content(void)const;
        uint get_number_of_repetitions(void)const;
        bool is_star(void)const;
        friend parser_result<bracketed_move> parse_bracketed_move(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<bracketed_move> parse_non_modifier(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<bracketed_move> parse_modifier(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);
        friend parser_result<bracketed_move>;
};

parser_result<bracketed_move> parse_bracketed_move(slice_iterator& it, const declarations& decls, messages_container& msg)throw(message);

#endif

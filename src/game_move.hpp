#ifndef GAME_MOVE
#define GAME_MOVE

#include<set>
#include<map>
#include<vector>
#include<ostream>

#include"token.hpp"
#include"parser_helpers.hpp"
#include"slice_iterator.hpp"
#include"message.hpp"
#include"game_order.hpp"
#include"options.hpp"

class moves_sum;

class atomic_move{
        int x;
        int y;
        std::set<token> on;
        std::set<token> off;
        bool every_on_legal : 1;
        bool no_off : 1;
        std::string make_splitter_name(void)const;
    public:
        atomic_move(void)noexcept;
        atomic_move(int x,int y,std::set<token>&& on,std::set<token>&& off);
        atomic_move(int x,int y,std::set<token>&& on_off, bool on_switch);
        atomic_move(int x,int y);

        friend parser_result<atomic_move> parse_atomic_move(
            slice_iterator& it,
            std::set<token>& encountered_pieces,
            messages_container& msg)throw(message);

        bool operator<(const atomic_move& m)const;
        bool operator==(const atomic_move& m)const;

        bool is_goal_eligible(void)const;
        friend std::ostream& operator<<(std::ostream& out,const atomic_move& m);
        atomic_move flatten(void); // moves out value
        bool can_absorb(const atomic_move& right_hand_side)const;
        void absorb(atomic_move&& right_hand_side);
        bool can_be_absorbed(const atomic_move& left_hand_side)const;
        void be_absorbed(atomic_move&& left_hand_side);
        bool has_off(void)const;
        bool is_in_place(void)const;
        std::pair<atomic_move,atomic_move> prepare_to_split(std::set<token>& known_pieces,std::set<token>& pieces_after_split,uint& current_id); // moves out value
        void to_semisteps(
            moves_sum& N,
            moves_sum& B,
            moves_sum& T,
            moves_sum& BT,
            const std::set<token>& splitters); // moves out value
};

parser_result<atomic_move> parse_atomic_move(slice_iterator& it, messages_container& msg)throw(message);
std::ostream& operator<<(std::ostream& out,const atomic_move& m);
std::ostream& operator<<(std::ostream& out,const std::set<token>& s);

class turn_change_indicator{
        token player;
        turn_change_indicator(const token& name)noexcept;
    public:
        turn_change_indicator(void)noexcept;

        friend parser_result<turn_change_indicator> parse_turn_change_indicator(
            slice_iterator& it,
            const game_order& players,
            int player_number,
            messages_container& msg)throw(message);

        bool operator<(const turn_change_indicator& m)const;
        bool operator==(const turn_change_indicator& m)const;

        bool is_goal_eligible(void)const;
        friend std::ostream& operator<<(std::ostream& out,const turn_change_indicator& m);
        turn_change_indicator flatten(void); // moves out value
        void to_semisteps(
            moves_sum& N,
            moves_sum& B,
            moves_sum& T,
            moves_sum& BT); // moves out value
};

std::ostream& operator<<(std::ostream& out,const turn_change_indicator& m);

parser_result<turn_change_indicator> parse_turn_change_indicator(
    slice_iterator& it,
    const game_order& players,
    int player_number,
    messages_container& msg)throw(message);

class moves_concatenation;

class bracketed_move{
        uint repetition_number;
        union{
            moves_sum* sum;
            atomic_move* atomic;
            turn_change_indicator* turn_changer;
        };
        uint tag : 2; // 0 -> sum, 1 -> atomic, else -> turn_changer
    public:
        bracketed_move(void)noexcept;
        bracketed_move(moves_sum&& src,uint repetition_number=1)noexcept;
        bracketed_move(atomic_move&& src,uint repetition_number=1)noexcept;
        bracketed_move(turn_change_indicator&& src,uint repetition_number=1)noexcept;
        bracketed_move(const bracketed_move& src)noexcept;
        bracketed_move& operator=(const bracketed_move& src)noexcept;
        bracketed_move(bracketed_move&& src)noexcept;
        bracketed_move& operator=(bracketed_move&& src)noexcept;
        ~bracketed_move(void)noexcept;

        friend parser_result<bracketed_move> parse_bracketed_move(
            slice_iterator& it,
            std::set<token>& encountered_pieces,
            const game_order& players,
            int player_number,
            bool& contains_turn_changer,
            messages_container& msg)throw(message);

        bool operator<(const bracketed_move& m)const;
        bool operator==(const bracketed_move& m)const;

        void set_repetition_number(uint rn);
        void set_star(void);
        bool is_goal_eligible(void)const;
        bool is_atomic_move(void)const;
        const atomic_move& show_atomic_move(void)const;
        atomic_move give_atomic_move(void); // moves out value
        void print_rbg(std::ostream& out,uint recurrence_depth)const;
        bracketed_move flatten(void); // moves out value
        bool is_single_concatenation(void)const;
        moves_concatenation give_single_concatenation(void); // moves out value
        bool is_single_sum(void)const;
        moves_sum give_single_sum(void); // moves out value
        bool is_similar_repetitionwise(uint repetition_number_above)const;
        bool can_absorb(const atomic_move& right_hand_side)const;
        void absorb(atomic_move&& right_hand_side);
        bool can_be_absorbed(const atomic_move& left_hand_side)const;
        void be_absorbed(atomic_move&& left_hand_side);
        std::vector<bracketed_move> prepare_to_split(
            std::set<token>& known_pieces,
            std::set<token>& pieces_after_split,
            uint& current_id,
            bool is_beginning,
            bool& is_end,
            bool& contains_splitter,
            const options& o); // moves out value
        void to_semisteps(
            moves_sum& N,
            moves_sum& B,
            moves_sum& T,
            moves_sum& BT,
            const std::set<token>& splitters); // moves out value
};

parser_result<bracketed_move> parse_bracketed_move(
    slice_iterator& it,
    std::set<token>& encountered_pieces,
    const game_order& players,
    int player_number,
    bool& contains_turn_changer,
    messages_container& msg)throw(message);

class moves_concatenation{
        std::vector<bracketed_move> content;
    public:
        moves_concatenation(std::vector<bracketed_move>&& src)noexcept;
        moves_concatenation(void)noexcept;

        friend parser_result<moves_concatenation> parse_moves_concatenation(
            slice_iterator& it,
            std::set<token>& encountered_pieces,
            const game_order& players,
            int player_number,
            bool& contains_turn_changer,
            messages_container& msg)throw(message);

        bool operator<(const moves_concatenation& m)const;
        bool operator==(const moves_concatenation& m)const;

        bool is_goal_eligible(void)const;
        void print_rbg(std::ostream& out,uint recurrence_depth)const;
        moves_concatenation flatten(void); // moves out value
        bool is_single_sum(void)const;
        moves_sum give_single_sum(void); // moves out value
        bool is_single_bracket(uint repetition_number_above)const;
        bracketed_move give_single_bracket(void); // moves out value
        bool can_absorb(const atomic_move& right_hand_side)const;
        void absorb(atomic_move&& right_hand_side);
        bool can_be_absorbed(const atomic_move& left_hand_side)const;
        void be_absorbed(atomic_move&& left_hand_side);
        std::vector<bracketed_move> move_out(void);
        moves_concatenation prepare_to_split(
            std::set<token>& known_pieces,
            std::set<token>& pieces_after_split,
            uint& current_id,
            bool is_beginning,
            bool& is_end,
            bool& contains_splitter,
            const options& o); // moves out value
        bool is_epsilon(void)const;
        void to_semisteps(
            moves_sum& N,
            moves_sum& B,
            moves_sum& T,
            moves_sum& BT,
            const std::set<token>& splitters); // moves out value
};

parser_result<moves_concatenation> parse_moves_concatenation(
    slice_iterator& it,
    std::set<token>& encountered_pieces,
    const game_order& players,
    int player_number,
    bool& contains_turn_changer,
    messages_container& msg)throw(message);

class moves_sum{
        std::vector<moves_concatenation> content;
    public:
        moves_sum(std::vector<moves_concatenation>&& src)noexcept;
        moves_sum(void)noexcept;

        friend parser_result<moves_sum> parse_moves_sum(
            slice_iterator& it,
            std::set<token>& encountered_pieces,
            const game_order& players,
            int player_number,
            bool& contains_turn_changer,
            messages_container& msg)throw(message);

        bool operator<(const moves_sum& m)const;
        bool operator==(const moves_sum& m)const;

        bool is_goal_eligible(void)const;
        void print_rbg(std::ostream& out,uint recurrence_depth)const;
        moves_sum flatten(void); // moves out value
        bool is_single_concatenation(void)const;
        moves_concatenation give_single_concatenation(void); // moves out value
        bool is_single_bracket(uint repetition_number_above)const;
        bracketed_move give_single_bracket(void); // moves out value
        bool can_absorb(const atomic_move& right_hand_side)const;
        void absorb(atomic_move&& right_hand_side);
        bool can_be_absorbed(const atomic_move& left_hand_side)const;
        void be_absorbed(atomic_move&& left_hand_side);
        moves_sum prepare_to_split(
            std::set<token>& known_pieces,
            std::set<token>& pieces_after_split,
            uint& current_id,
            bool is_beginning,
            bool& is_end,
            bool& contains_splitter,
            const options& o); // moves out value
        bool is_epsilon(void)const;
        bool is_empty(void)const;
        void concat_move(moves_sum&& m);
        void set_star(void);
        void add_move(moves_sum&& m);
        void to_semisteps(
            moves_sum& N,
            moves_sum& B,
            moves_sum& T,
            moves_sum& BT,
            const std::set<token>& splitters); // moves out value
};

parser_result<moves_sum> parse_moves_sum(
    slice_iterator& it,
    std::set<token>& encountered_pieces,
    const game_order& players,
    int player_number,
    bool& contains_turn_changer,
    messages_container& msg)throw(message);
moves_sum epsilon(void);
moves_sum empty_expression(void);
moves_sum single_letter(atomic_move&& m);
moves_sum single_letter(turn_change_indicator&& m);

void print_spaces(std::ostream& out,uint n);

#endif

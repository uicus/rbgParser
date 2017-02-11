#ifndef GAME_GOAL
#define GAME_GOAL

#include<set>
#include<map>
#include<ostream>

#include"token.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"game_move.hpp"

class atomic_goal{
        token first_value;
        token second_value;
        uint kind_of_comparison : 3; // 0 -> '<', 1 -> '<=', 2 -> '=', 3 -> '>=', 4 -> '>'
    public:
        atomic_goal(void)noexcept;
        atomic_goal(token&& first,token&& second,uint kind);

        friend parser_result<atomic_goal> parse_atomic_goal(
            slice_iterator& it,
            const std::set<token>& encountered_pieces,
            messages_container& msg)throw(message);

        bool operator==(const atomic_goal& m)const;
        bool operator<(const atomic_goal& m)const;
        friend std::ostream& operator<<(std::ostream& out,const atomic_goal& g);

        void gather_information(
            int& max_turn_number,
            std::map<token,std::set<int>>& possible_comparisons,
            std::set<token>& should_count,
            uint board_size)const;
};

std::ostream& operator<<(std::ostream& out,const atomic_goal& g);

parser_result<atomic_goal> parse_atomic_goal(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
    messages_container& msg)throw(message);

class piece_placement_goal{
        token piece;
        uint x;
        uint y;
    public:
        piece_placement_goal(void)noexcept;
        piece_placement_goal(token&& piece,uint x,uint y);

        friend parser_result<piece_placement_goal> parse_piece_placement_goal(
            slice_iterator& it,
            const std::set<token>& encountered_pieces,
            uint board_height,
            uint board_width,
            messages_container& msg)throw(message);

        bool operator==(const piece_placement_goal& m)const;
        bool operator<(const piece_placement_goal& m)const;
        friend std::ostream& operator<<(std::ostream& out,const piece_placement_goal& g);
};

std::ostream& operator<<(std::ostream& out,const piece_placement_goal& g);

parser_result<piece_placement_goal> parse_piece_placement_goal(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
    uint board_height,
    uint board_width,
    messages_container& msg)throw(message);

class goals_alternative;
class goals_conjunction;

class negatable_goal{
        bool negated;
        union{
            goals_alternative* alternative;
            atomic_goal* atomic;
            moves_sum* move_goal;
            piece_placement_goal* piece_placement;
        };
        uint tag : 2; // 0 -> alternative, 1 -> atomic, 2 -> move_goal, else -> piece_placement
    public:
        negatable_goal(goals_alternative&& src)noexcept;
        negatable_goal(atomic_goal&& src)noexcept;
        negatable_goal(moves_sum&& src)noexcept;
        negatable_goal(piece_placement_goal&& src)noexcept;
        negatable_goal(void)noexcept;
        negatable_goal(const negatable_goal& src)noexcept;
        negatable_goal& operator=(const negatable_goal& src)noexcept;
        negatable_goal(negatable_goal&& src)noexcept;
        negatable_goal& operator=(negatable_goal&& src)noexcept;
        ~negatable_goal(void)noexcept;

        friend parser_result<negatable_goal> parse_negatable_goal(
            slice_iterator& it,
            const std::set<token>& encountered_pieces,
            uint board_height,
            uint board_width,
            messages_container& msg)throw(message);

        bool operator==(const negatable_goal& m)const;
        bool operator<(const negatable_goal& m)const;

        void negate(void);
        void print_rbg(std::ostream& out,uint recurrence_depth)const;
        void apply_negation(bool should_be_negated=false);
        bool is_single_alternative(void)const;
        goals_alternative give_single_alternative(void); // moves out value
        bool is_single_conjunction(void)const;
        goals_conjunction give_single_conjunction(void); // moves out value
        negatable_goal flatten(void); // moves out value

        void gather_information(
            int& max_turn_number,
            std::map<token,std::set<int>>& possible_comparisons,
            std::set<token>& should_count,
            uint board_size)const;
};

parser_result<negatable_goal> parse_negatable_goal(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
    uint board_height,
    uint board_width,
    messages_container& msg)throw(message);

class goals_conjunction{
        std::vector<negatable_goal> content;
    public:
        goals_conjunction(std::vector<negatable_goal>&& src)noexcept;
        goals_conjunction(negatable_goal&& src)noexcept;
        goals_conjunction(void)noexcept;

        friend parser_result<goals_conjunction> parse_goals_conjunction(
            slice_iterator& it,
            const std::set<token>& encountered_pieces,
            uint board_height,
            uint board_width,
            messages_container& msg)throw(message);

        bool operator==(const goals_conjunction& m)const;
        bool operator<(const goals_conjunction& m)const;
        void print_rbg(std::ostream& out,uint recurrence_depth)const;
        void apply_negation(bool should_be_negated=false);
        bool is_single_negation(void)const;
        negatable_goal give_single_negation(void); // moves out value
        bool is_single_alternative(void)const;
        goals_alternative give_single_alternative(void); // moves out value
        goals_conjunction flatten(void); // moves out value

        void gather_information(
            int& max_turn_number,
            std::map<token,std::set<int>>& possible_comparisons,
            std::set<token>& should_count,
            uint board_size)const;
};

parser_result<goals_conjunction> parse_goals_conjunction(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
    uint board_height,
    uint board_width,
    messages_container& msg)throw(message);

class goals_alternative{
        std::vector<goals_conjunction> content;
    public:
        goals_alternative(std::vector<goals_conjunction>&& src)noexcept;
        goals_alternative(goals_conjunction&& src)noexcept;
        goals_alternative(void)noexcept;

        friend parser_result<goals_alternative> parse_goals_alternative(
            slice_iterator& it,
            const std::set<token>& encountered_pieces,
            uint board_height,
            uint board_width,
            messages_container& msg,
            bool can_be_empty)throw(message);

        bool operator==(const goals_alternative& m)const;
        bool operator<(const goals_alternative& m)const;
        void print_rbg(std::ostream& out,uint recurrence_depth)const;
        friend std::ostream& operator<<(std::ostream& out,const goals_alternative& g);
        void apply_negation(bool should_be_negated=false);
        bool is_single_conjunction(void)const;
        goals_conjunction give_single_conjunction(void); // moves out value
        bool is_single_negation(void)const;
        negatable_goal give_single_negation(void); // moves out value
        goals_alternative flatten(void); // moves out value

        void gather_information(
            int& max_turn_number,
            std::map<token,std::set<int>>& possible_comparisons,
            std::set<token>& should_count,
            uint board_size)const;
};

std::ostream& operator<<(std::ostream& out,const goals_alternative& g);

parser_result<goals_alternative> parse_goals_alternative(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
    uint board_height,
    uint board_width,
    messages_container& msg,
    bool can_be_empty=false)throw(message);

#endif

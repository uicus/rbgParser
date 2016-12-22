#ifndef GAME_GOAL
#define GAME_GOAL

#include<set>
#include<ostream>

#include"token.hpp"
#include"message.hpp"
#include"slice_iterator.hpp"
#include"parser_helpers.hpp"
#include"game_move.hpp"

class atomic_goal{
        token compared_value;
        uint compared_const;
        uint kind_of_comparison : 3; // 0 -> '<', 1 -> '<=', 2 -> '=', 3 -> '>=', 4 -> '>'
    public:
        atomic_goal(void)noexcept;
        atomic_goal(token&& t,uint n,uint kind);

        friend parser_result<atomic_goal> parse_atomic_goal(
            slice_iterator& it,
            const std::set<token>& encountered_pieces,
            messages_container& msg)throw(message);

        bool operator==(const atomic_goal& m)const;
        bool operator<(const atomic_goal& m)const;
        friend std::ostream& operator<<(std::ostream& out,const atomic_goal& g);
};

std::ostream& operator<<(std::ostream& out,const atomic_goal& g);

parser_result<atomic_goal> parse_atomic_goal(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
    messages_container& msg)throw(message);

class goals_alternative;

class negatable_goal{
        bool negated;
        union{
            goals_alternative* alternative;
            atomic_goal* atomic;
            moves_sum* move_goal;
        };
        uint tag : 2; // 0 -> alternative, 1 -> atomic, else -> move_goal
        negatable_goal(goals_alternative&& src)noexcept;
        negatable_goal(atomic_goal&& src)noexcept;
        negatable_goal(moves_sum&& src)noexcept;
    public:
        negatable_goal(void)noexcept;
        negatable_goal(const negatable_goal& src)noexcept;
        negatable_goal& operator=(const negatable_goal& src)noexcept;
        negatable_goal(negatable_goal&& src)noexcept;
        negatable_goal& operator=(negatable_goal&& src)noexcept;
        ~negatable_goal(void)noexcept;

        friend parser_result<negatable_goal> parse_negatable_goal(
            slice_iterator& it,
            const std::set<token>& encountered_pieces,
            messages_container& msg)throw(message);

        bool operator==(const negatable_goal& m)const;
        bool operator<(const negatable_goal& m)const;

        void negate(void);
        void print_rbg(std::ostream& out,uint recurrence_depth)const;
};

parser_result<negatable_goal> parse_negatable_goal(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
    messages_container& msg)throw(message);

class goals_conjunction{
        std::set<negatable_goal> content;
        goals_conjunction(std::set<negatable_goal>&& src)noexcept;
    public:
        goals_conjunction(void)noexcept;

        friend parser_result<goals_conjunction> parse_goals_conjunction(
            slice_iterator& it,
            const std::set<token>& encountered_pieces,
            messages_container& msg)throw(message);

        bool operator==(const goals_conjunction& m)const;
        bool operator<(const goals_conjunction& m)const;
        void print_rbg(std::ostream& out,uint recurrence_depth)const;
};

parser_result<goals_conjunction> parse_goals_conjunction(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
    messages_container& msg)throw(message);

class goals_alternative{
        std::set<goals_conjunction> content;
        goals_alternative(std::set<goals_conjunction>&& src)noexcept;
    public:
        goals_alternative(void)noexcept;

        friend parser_result<goals_alternative> parse_goals_alternative(
            slice_iterator& it,
            const std::set<token>& encountered_pieces,
            messages_container& msg,
            bool can_be_empty)throw(message);

        bool operator==(const goals_alternative& m)const;
        bool operator<(const goals_alternative& m)const;
        void print_rbg(std::ostream& out,uint recurrence_depth)const;
        friend std::ostream& operator<<(std::ostream& out,const goals_alternative& g);
};

std::ostream& operator<<(std::ostream& out,const goals_alternative& g);

parser_result<goals_alternative> parse_goals_alternative(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
    messages_container& msg,
    bool can_be_empty=false)throw(message);

#endif

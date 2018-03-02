#ifndef TOKEN
#define TOKEN

#include<string>
#include<vector>
#include<set>

#include"message.hpp"

namespace rbg_parser{

enum token_type{
    left_round_bracket=0,
    left_question_bracket,
    left_exclamation_bracket,
    right_round_bracket,
    left_square_bracket,
    left_lazy_bracket,
    right_square_bracket,
    tilde,
    hash,
    minus,
    plus,
    greater_plus,
    double_plus,
    double_minus,
    caret,
    star,
    comma,
    semicolon,
    dollar,
    at_sign,
    equal,
    arrow,
    exclamation,
    question,
    not_equal,
    double_equal,
    less,
    less_equal,
    greater,
    greater_equal,
    logical_and,
    logical_or,
    logical_not,
    bound,
    player,
    game,
    players,
    pieces,
    variables,
    rules,
    finalizer,
    board,
    number,
    identifier, // must third-to-last
    quotation,
    dummy
};

class token{
        uint position_in_file;
        token_type type;
        union{
            std::string* contained_string;
            uint number_value;
        };
        token(uint position, token_type type)noexcept;
    public:
        token(void)noexcept;
        token(const token& src)noexcept;
        token(token&& src)noexcept;
        token& operator=(const token& src)noexcept;
        token& operator=(token&& src)noexcept;
        token(std::string&& name)noexcept;
        ~token(void)noexcept;

        friend std::set<token> create_tokens_set(uint position);
        friend token create_quotation(uint position);
        std::string to_string(void)const;
        std::string get_string_content(void)const;
        uint get_value(void)const;
        void increase(void);
        token_type get_type(void)const;
        bool could_be_next_character(char next_character, uint position)const;
        bool contains_exactly_n_chars(uint n)const;
        void take_next_character(char next_character, messages_container& msg)throw(message);
        uint get_position(void)const;
        void reverse_comparison(void);
        token& operator+=(const token& t)throw(std::string);

        bool operator==(const token& t)const;
        bool operator<(const token& t)const;
};

std::set<token> create_tokens_set(uint position);
token create_quotation(uint position);
uint seek_end_of_quotation(const std::string& input, uint current_char, token& quote, messages_container& msg);
uint seek_end_of_singleline_comment(const std::string& input, uint current_char, messages_container& msg);
uint seek_end_of_multiline_comment(const std::string& input, uint current_char, messages_container& msg);
uint skip_whitespace(const std::string& input, uint current_char, messages_container& msg);
bool filter_set_with_character(std::set<token>& current_set, char next_character, uint position, messages_container& msg)throw(message); // returns whether char should be re-parsed and current_set contains full parsed token
std::vector<token> tokenize(const std::string& input, messages_container& msg)throw(message);

}

#endif

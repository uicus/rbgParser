#ifndef DECLARATIONS
#define DECLARATIONS

#include<set>
#include<string>

#include"token.hpp"

class declarations{
        std::set<token> players_names;
        std::set<token> pieces_names;
        std::set<token> variables_names;
    public:
        declarations(std::set<token>&& players_names, std::set<token>&& pieces_names, std::set<token>&& variables_names);
        const std::set<token>& get_legal_pieces(void)const;
        const std::set<token>& get_legal_players(void)const;
        const std::set<token>& get_legal_variables(void)const;
        std::string to_rbg(void)const;
};

#endif

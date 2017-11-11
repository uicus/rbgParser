#ifndef DECLARATIONS
#define DECLARATIONS

#include<set>

#include"token.hpp"

class declarations{
        std::set<token> players_names;
        std::set<token> pieces_names;
        std::set<token> variables_names;
    public:
        declarations(std::set<token>&& players_names, std::set<token>&& pieces_names, std::set<token>&& variables_names);
        const std::set<token>& get_legal_pieces(void)const;
};

#endif

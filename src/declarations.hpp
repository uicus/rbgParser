#ifndef DECLARATIONS
#define DECLARATIONS

#include<set>
#include<string>

#include"token.hpp"

namespace rbg_parser{

class declarations{
        std::set<token> players_names;
        std::set<token> pieces_names;
        std::set<token> variables_names;
        std::set<token> edges_names;
        token first_player;
    public:
        declarations(std::set<token>&& players_names, std::set<token>&& pieces_names, std::set<token>&& variables_names, token&& first_player);
        const std::set<token>& get_legal_pieces(void)const;
        const std::set<token>& get_legal_players(void)const;
        const std::set<token>& get_legal_variables(void)const;
        const std::set<token>& get_legal_edges(void)const;
        void add_edge_label(const token& name);
        const token& get_first_player(void)const;
        std::string to_rbg(void)const;
};

}

#endif

#ifndef UNCHECKED_GRAPH
#define UNCHECKED_GRAPH

#include<map>
#include"token.hpp"
#include"slice_iterator.hpp"

namespace rbg_parser{

class declarations;
class messages_container;
class message;
class slice_iterator;
class graph;

class unchecked_graph{
        typedef std::pair<slice_iterator, token> neighbor_name;
        typedef std::map<token, neighbor_name> neighbors;
        std::map<token, std::pair<token, neighbors>> vertices;
        token first_vertex_name;
        std::map<token, uint> create_name_number_correspondence(void)const;
        std::map<token, uint> check_and_transform_edges(
            const neighbors& n,
            const std::map<token, uint>& name_number_correspondence,
            messages_container& msg)const throw(message);
    public:
        void add_vertex(const token& name, token&& starting_piece);
        bool vertex_exists(const token& name)const;
        void add_edge(const token& source_vertex, const token& target_vertex, const slice_iterator& target_position, const token& edge_label);
        bool edge_exists(const token& source_vertex, const token& edge_label)const;
        graph check_vertices_consistency(messages_container& msg)const throw(message);
};

bool parse_vertex(unchecked_graph& g, declarations& decl, slice_iterator& it, messages_container& msg)throw(message);

}

#endif

#include"unchecked_graph.hpp"
#include<cassert>

namespace rbg_parser{

void unchecked_graph::add_vertex(const token& name, const token& starting_piece){
    assert(not vertex_exists(name));
    vertices.insert(std::make_pair(name, std::make_pair(starting_piece, neighbors())));
}

bool unchecked_graph::vertex_exists(const token& name)const{
    return vertices.find(name) != vertices.end();
}

void unchecked_graph::add_edge(
    const token& source_vertex,
    const token& target_vertex, const slice_iterator& target_position,
    const token& edge_label, const slice_iterator& label_position){
    assert(vertex_exists(source_vertex) && not edge_exists(source_vertex, edge_label));
    auto prepared_neighbor = std::make_pair(target_position, target_vertex);
    auto& existing_neighbors = vertices.at(source_vertex).second;
    existing_neighbors.insert(std::make_pair(edge_label, std::make_pair(label_position, std::move(prepared_neighbor))));
}

bool unchecked_graph::edge_exists(const token& source_vertex, const token& edge_label)const{
    if(not vertex_exists(source_vertex))
        return false;
    const auto& existing_neighbors = vertices.at(source_vertex).second;
    return existing_neighbors.find(edge_label) != existing_neighbors.end();
}

}

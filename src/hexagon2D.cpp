#include"hexagon2D.hpp"
#include"declarations.hpp"

namespace rbg_parser{

void hexagon2D::parse_edge_argument(token hexagon2D::*direction, declarations& decl, slice_iterator& it, messages_container& msg){
    this->*direction = parse_edge_name(decl,it,msg);
    if(it.current(msg).get_type() != comma)
        throw msg.build_message(it.create_call_stack("Expected \',\', encountered \'"+it.current(msg).to_string()+"\'"));
    it.next(msg);
}

bool hexagon2D::add_line_if_aligned(std::vector<token>&& line){
    if(starting_pieces.empty()){
        starting_pieces.push_back(std::move(line));
        return true;
    }
    else if(has_peak_line){
        if(starting_pieces.back().size()-1 == line.size()){
            starting_pieces.push_back(std::move(line));
            return true;
        }
        else
            return false;
    }
    else{
        if(starting_pieces.back().size()-1 == line.size()){
            has_peak_line = true;
            peak_line = starting_pieces.size()-1;
            starting_pieces.push_back(std::move(line));
            return true;
        }
        else if(starting_pieces.back().size()+1 == line.size()){
            starting_pieces.push_back(std::move(line));
            return true;
        }
        else
            return false;
    }
}

bool hexagon2D::parse_boardline(declarations& decl, slice_iterator& it, messages_container& msg){
    if(not it.has_value() or it.current(msg).get_type() != left_square_bracket)
        return false;
    auto beginning = it;
    it.next(msg);
    auto pieces_sequence = parse_sequence(it,"boardline",decl.get_legal_pieces(),true,msg);
    if(not pieces_sequence.is_success())
        throw msg.build_message(it.create_call_stack("Expected pieces sequence, encountered \'"+it.current(msg).to_string()+"\'"));
    if(pieces_sequence.get_value().empty())
        throw msg.build_message(beginning.create_call_stack("Boardline cannot be empty"));
    if(not add_line_if_aligned(pieces_sequence.move_value())){
        if(has_peak_line)
            throw msg.build_message(beginning.create_call_stack("This line should be one square shorter than previous"));
        else
            throw msg.build_message(beginning.create_call_stack("This line should be one square shorter or longer than previous"));
    }
    if(it.current(msg).get_type() != right_square_bracket)
        throw msg.build_message(it.create_call_stack("Expected \']\', encountered \'"+it.current(msg).to_string()+"\'"));
    it.next(msg);
    return true;
}

uint hexagon2D::trapezoid_number(uint base, uint line_number)const{
    return line_number*base+line_number*(line_number-1)/2;
}

uint hexagon2D::compute_index(uint line_number, uint column_number)const{
    uint first_line_length = starting_pieces[0].size();
    if(not has_peak_line or line_number<=peak_line+1)
        return trapezoid_number(first_line_length,line_number)+column_number;
    else
        return trapezoid_number(first_line_length,peak_line+1)
             + trapezoid_number(starting_pieces[line_number-1].size(),line_number-peak_line-1)
             + column_number;
}

bool hexagon2D::below_peak_line(uint line_number)const{
    return has_peak_line and line_number > peak_line;
}

bool hexagon2D::above_peak_line(uint line_number)const{
    return not has_peak_line or line_number < peak_line;
}

std::tuple<token,token,edges> hexagon2D::transform_square(uint line_number, uint column_number)const{
    token vertex_name("x"+std::to_string(column_number)+"y"+std::to_string(line_number));
    token starting_piece = starting_pieces[line_number][column_number];
    edges outgoing_edges;
    if(line_number>0){
        if(column_number<starting_pieces[line_number].size()-1 or below_peak_line(line_number))
            outgoing_edges.insert(std::make_pair(ne,compute_index(line_number-1,column_number+(below_peak_line(line_number)?1:0))));
        if(column_number>0 or below_peak_line(line_number))
            outgoing_edges.insert(std::make_pair(nw,compute_index(line_number-1,column_number-(below_peak_line(line_number)?0:1))));
    }
    if(line_number<starting_pieces.size()-1){
        if(column_number<starting_pieces[line_number].size()-1 or above_peak_line(line_number))
            outgoing_edges.insert(std::make_pair(se,compute_index(line_number+1,column_number+(above_peak_line(line_number)?1:0))));
        if(column_number>0 or above_peak_line(line_number))
            outgoing_edges.insert(std::make_pair(sw,compute_index(line_number+1,column_number-(above_peak_line(line_number)?0:1))));
    }
    if(column_number<starting_pieces[line_number].size()-1)
        outgoing_edges.insert(std::make_pair(e,compute_index(line_number,column_number+1)));
    if(column_number>0)
        outgoing_edges.insert(std::make_pair(w,compute_index(line_number,column_number-1)));
    return std::make_tuple(std::move(vertex_name),std::move(starting_piece),std::move(outgoing_edges));
}

graph hexagon2D::build_graph(messages_container&)const{
    std::vector<std::tuple<token,token,edges>> vertices;
    for(uint i=0;i<starting_pieces.size();++i)
        for(uint j=0;j<starting_pieces[i].size();++j)
            vertices.push_back(transform_square(i,j));
    return graph(std::move(vertices));
}

parser_result<std::unique_ptr<graph_builder>> parse_hexagon2D(declarations& decl, slice_iterator& it, messages_container& msg){
    if(not it.has_value() or it.current(msg).get_type() != hexagon)
        return failure<std::unique_ptr<graph_builder>>();
    it.next(msg);
    if(it.current(msg).get_type() != left_round_bracket)
        throw msg.build_message(it.create_call_stack("Expected \'(\', encountered \'"+it.current(msg).to_string()+"\'"));
    it.next(msg);
    hexagon2D result;
    result.parse_edge_argument(&hexagon2D::nw,decl,it,msg);
    result.parse_edge_argument(&hexagon2D::ne,decl,it,msg);
    result.parse_edge_argument(&hexagon2D::e,decl,it,msg);
    result.parse_edge_argument(&hexagon2D::se,decl,it,msg);
    result.parse_edge_argument(&hexagon2D::sw,decl,it,msg);
    result.parse_edge_argument(&hexagon2D::w,decl,it,msg);
    if(not result.parse_boardline(decl,it,msg))
        throw msg.build_message(it.create_call_stack("\'hexagon\' generator requires at least one boardline"));
    while(result.parse_boardline(decl,it,msg));
    if(it.current(msg).get_type() != right_round_bracket)
        throw msg.build_message(it.create_call_stack("Expected \')\', encountered \'"+it.current(msg).to_string()+"\'"));
    if(it.next(msg))
        msg.add_message(it.create_call_stack("Unexpected tokens at the end of \'board\' section"));
    return success(std::unique_ptr<graph_builder>(new hexagon2D(std::move(result))));
}

}

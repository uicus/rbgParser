#include"parsed_game.hpp"
#include"gdl_constants.hpp"

#include<set>

parsed_game::parsed_game(
    std::string&& n,
    game_board&& b,
    std::map<token,moves_sequence>&& m,
    std::map<token,goals_alternative>&& g,
    game_order&& pl,
    std::set<token>&& kp)noexcept:
name(std::move(n)),
brd(std::move(b)),
players(std::move(pl)),
moves(std::move(m)),
goals(std::move(g)),
known_pieces(std::move(kp)){}

void parsed_game::to_simple(const options& o){
    uint current_id = 0;
    for(auto& el:moves){
        std::set<token> splitters;
        el.second.flatten();
        el.second.prepare_to_split(known_pieces,splitters,current_id,o);
        el.second.split_into_semisteps(splitters);
    }
    for(auto& el:goals){
        el.second.apply_negation();
        el.second = el.second.flatten();
    }
}

std::ostream& operator<<(std::ostream& out,const parsed_game& g){
    out<<"#game \""<<g.name<<"\"\n\n";
    out<<g.brd<<'\n';
    out<<g.players<<'\n';
    for(const auto& el: g.moves)
        out<<"#player "<<el.first.to_string()<<'\n'<<el.second<<'\n';
    for(const auto& el: g.goals)
        out<<"#goal "<<el.first.to_string()<<'\n'<<el.second<<'\n';
    return out;
}

void parsed_game::print_pieces(std::ostream& out,const options& o)const{
    if(o.printing_comments())
        out<<section_title("Pieces")<<'\n';
    for(const auto& el: known_pieces)
        out<<"(pieceType "+el.to_string()+")\n";
    out<<'\n';
    // TODO
}

void parsed_game::print_base(std::ostream& out,const options& o,int max_turn)const{
    if(o.printing_comments())
        out<<section_title("Base")<<'\n';
    out<<"(<= (base (cell ?x ?y ?piece))\n    (file ?x)\n    (rank ?y)\n    (pieceType ?piece))\n";
    if(max_turn>=0){
        out<<"(base (step";
        uint l = length_of(uint(max_turn));
        for(uint i=0;i<l;++i)
            out<<" 0";
        out<<"))\n";
        out<<"(<= (base (step";
        for(uint i=0;i<l;++i)
            out<<" ?next"<<i;
        out<<"))\n    (stepSucc";
        for(uint i=0;i<l;++i)
            out<<" ?s"<<i;
        for(uint i=0;i<l;++i)
            out<<" ?next"<<i;
        out<<"))\n";
    }
    out<<'\n';
}

void parsed_game::print_initial_state(std::ostream& out,const options& o,int max_turn)const{
    if(o.printing_comments())
        out<<section_title("Initial state")<<'\n';
    brd.print_initial_setting(out);
    if(max_turn>=0){
        out<<"(init (step";
        uint l = length_of(uint(max_turn));
        for(uint i=0;i<l;++i)
            out<<" 0";
        out<<"))\n";
    }
    out<<'\n';
}

void parsed_game::print_turn_counter(std::ostream& out,const options& o,int max_turn, int max_equivalency)const{
    if(o.printing_comments())
        out<<section_title("Turns counter")<<'\n';
    out<<succ("stepSucc",max_turn,true)<<'\n';
    if(max_equivalency>=0)
        out<<unary_binary_equivalency("equivalent",max_turn,max_equivalency)<<'\n';
    out<<'\n';
}

void parsed_game::to_gdl(std::ostream& out,const options& o)const{
    int max_turn=-1;
    int max_turn_pieces_equivalency=-1;
    std::map<token,std::set<int>> possible_comparisons;
    std::set<token> should_count;
    for(const auto& el: goals)
        el.second.gather_information(max_turn,max_turn_pieces_equivalency,possible_comparisons,should_count,brd.get_height()*brd.get_width());
    if(o.printing_comments())
        out<<section_title(std::string(name))<<'\n';
    players.print_roles(out,o);
    if(o.printing_base())
        print_base(out,o,max_turn);
    print_initial_state(out,o,max_turn);
    brd.print_files_and_ranks(out,o);
    print_pieces(out,o);
    if(max_turn>=0)
        print_turn_counter(out,o,max_turn,max_turn_pieces_equivalency);
}

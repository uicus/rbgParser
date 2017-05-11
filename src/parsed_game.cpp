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
    if(players.get_number_of_players()>1)
        out<<"(<= (base (control ?p))\n    (role ?p))\n";
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
    if(players.get_number_of_players()>1)
        out<<"(init (control "<<players.get_player_name(0).to_string()<<")\n";
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

void parsed_game::print_moves(std::ostream& out,const options& o,uint& next_free_id)const{
    if(o.printing_comments())
        out<<section_title("Legal moves")<<'\n';
    out<<'\n';
    if(players.get_number_of_players()>1)
        out<<"(<= (legal ?player noop)\n"
           <<"    (role ?player)\n"
           <<"    (not (true (control ?player))))\n\n";
    good_pieces_sets s;
    for(const auto& el: moves){
        out<<"(<= (legal "<<el.first.to_string()<<" (move ?x ?y ?offFirst ?xLast ?yLast ?offLast ?nextPlayer))\n";
        if(players.get_number_of_players()>1)
            out<<"    (true (control ?player))\n";
        out<<"    (movesSeq_"<<el.first.to_string()<<" ?x ?y ?offFirst ?xLast ?yLast ?offLast ?nextPlayer))\n\n";
        el.second.write_as_gdl(out,s,"movesSeq_"+el.first.to_string(),next_free_id,o);
        out<<'\n';
    }
    s.print_all_sets(out);
    out<<"(<= (samePiece ?p ?p)\n"
       <<"    (pieceType ?p))\n\n";
    out<<"(noOff "<<no_off<<")\n\n";
    out<<"(<= (samePlayer ?p ?p)\n"
       <<"    (role ?p))\n\n";
    out<<"(semiStep "<<semi_turn<<")\n\n";
}

void parsed_game::print_arithmetics(std::ostream& out,const options& o,int max_n)const{
    if(o.printing_comments())
        out<<section_title("Arithmetics")<<'\n';
    out<<'\n';
    for(int i=0;i<max_n;++i)
        out<<"(arithSucc "<<i<<' '<<i+1<<")\n";
    out<<'\n';
    out<<"(<= (sum ?x 0 ?x)\n"
	   <<"    (arithSucc ?x ?y))\n\n"
	   <<"(<= (sum ?x 0 ?x)\n"
	   <<"    (arithSucc ?y ?x))\n\n"
       <<"(<= (sum ?x ?y ?z)\n"
	   <<"    (arithSucc ?x ?succx)\n"
	   <<"    (arithSucc ?prevy ?y)\n"
	   <<"    (sum ?succx ?prevy ?z))\n\n"
       <<"(<= (sub ?x ?y ?z)\n"
       <<"    (sum ?z ?y ?x))\n\n"
       <<"(<= (equal ?x ?x)\n"
       <<"    (sum ?x 0 ?x))\n\n";
}

void parsed_game::to_gdl(std::ostream& out,const options& o)const{
    int max_turn=-1;
    int max_turn_pieces_equivalency=-1;
    std::map<token,std::set<int>> possible_comparisons;
    std::set<token> should_count;
    int max_repetition = -1;
    for(const auto& el: moves){
        int m = el.second.max_repetition(o);
        if((m>max_repetition&&max_repetition!=0)||(m==0&&o.is_prolog_safe()))
            max_repetition = m;
    }
    for(const auto& el: goals)
        el.second.gather_information(max_turn,max_turn_pieces_equivalency,max_repetition,possible_comparisons,should_count,brd.get_height()*brd.get_width(),o);
    if(o.printing_comments())
        out<<section_title(std::string(name))<<'\n';
    players.print_roles(out,o);
    if(o.printing_base())
        print_base(out,o,max_turn);
    print_initial_state(out,o,max_turn);
    brd.print_files_and_ranks(out,o);
    print_pieces(out,o);
    print_arithmetics(out,o,std::max(max_repetition,int(std::max(brd.get_width(),brd.get_height()))));
    if(max_turn>=0)
        print_turn_counter(out,o,max_turn,max_turn_pieces_equivalency);
    uint next_free_id = 0;
    print_moves(out,o,next_free_id);
}

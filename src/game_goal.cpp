#include"game_goal.hpp"

#include<cassert>

atomic_goal::atomic_goal(void)noexcept:
first_value(), // dummy
second_value(), // dummy
kind_of_comparison(0){}

atomic_goal::atomic_goal(token&& first,token&& second,uint kind):
first_value(std::move(first)),
second_value(std::move(second)),
kind_of_comparison(kind){
    if(second_value.get_type()==number&&(kind_of_comparison==4||kind_of_comparison==1)){
        second_value.increase();
        --kind_of_comparison;
    }
    else if(first_value.get_type()==number&&(kind_of_comparison==3||kind_of_comparison==0)){
        first_value.increase();
        ++kind_of_comparison;
    }
}

parser_result<atomic_goal> parse_atomic_goal(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
    messages_container& msg)throw(message){
    if(!it.has_value())
        return failure<atomic_goal>();
    parser_result<token> first_variable = parse_variable(it,encountered_pieces,msg);
    uint kind;
    if(!first_variable.is_success())
        return failure<atomic_goal>();
    if(!it.has_value())
        throw msg.build_message("Unexpected end of goal");
    if(it.current().get_type()==less)
        kind = 0;
    else if(it.current().get_type()==less_equal)
        kind = 1;
    else if(it.current().get_type()==equal)
        kind = 2;
    else if(it.current().get_type()==greater_equal)
        kind = 3;
    else if(it.current().get_type()==greater)
        kind = 4;
    else
        throw msg.build_message(it.create_call_stack("Expected \'<\', \'<=\', \'=\', \'>=\' or \'>\', encountered \'"+it.current().to_string()+"\'"));
    if(!it.next(msg))
        throw msg.build_message("Unexpected end of goal");
    parser_result<token> second_variable = parse_variable(it,encountered_pieces,msg);
    if(!second_variable.is_success())
        throw msg.build_message(it.create_call_stack("Expected variable or non-negative integer, encountered \'"+it.current().to_string()+"\'"));
    return success(atomic_goal(first_variable.move_value(),second_variable.move_value(),kind));
}

bool atomic_goal::operator==(const atomic_goal& m)const{
    return first_value==m.first_value && second_value==m.second_value && kind_of_comparison==m.kind_of_comparison;
}

bool atomic_goal::operator<(const atomic_goal& m)const{
    return first_value<m.first_value
        || (first_value==m.first_value && second_value<m.second_value)
        || (first_value==m.first_value && second_value==m.second_value && kind_of_comparison<m.kind_of_comparison);
}

std::ostream& operator<<(std::ostream& out,const atomic_goal& g){
    if(g.first_value.get_type()==number)
        out<<g.first_value.get_value();
    else
        out<<'$'<<g.first_value.to_string();
    switch(g.kind_of_comparison){
    case 0:
        out<<'<';
        break;
    case 1:
        out<<"<=";
        break;
    case 2:
        out<<'=';
        break;
    case 3:
        out<<">=";
        break;
    default:
        out<<'>';
    }
    if(g.second_value.get_type()==number)
        out<<g.second_value.get_value();
    else
        out<<'$'<<g.second_value.to_string();
    return out;
}

void atomic_goal::gather_information(
    int& max_turn_number,
    std::map<token,std::set<int>>& possible_comparisons,
    std::set<token>& should_count,
    uint board_size)const{
    if(first_value.get_type()==turn){
        if(second_value.get_type()!=number){
            max_turn_number = std::max(max_turn_number,int(kind_of_comparison==4||kind_of_comparison==1?board_size+1:board_size));
        }
        else
            max_turn_number = std::max(max_turn_number,int(second_value.get_value()));
        if(second_value.get_type()==identifier)
            should_count.insert(second_value);
    }
    else if(first_value.get_type()==identifier){
        if(second_value.get_type()==identifier){
            should_count.insert(first_value);
            should_count.insert(second_value);
        }
        else if(second_value.get_type()==turn){
            max_turn_number = std::max(max_turn_number,int(kind_of_comparison==4||kind_of_comparison==1?board_size+1:board_size));
            should_count.insert(first_value);
        }
        else{
            if(possible_comparisons.count(first_value)==0)
                possible_comparisons[first_value] = std::set<int>();
            possible_comparisons[first_value].insert(second_value.get_value());
        }
    }
    else{
        if(second_value.get_type()==identifier){
            if(possible_comparisons.count(second_value)==0)
                possible_comparisons[second_value] = std::set<int>();
            possible_comparisons[second_value].insert(first_value.get_value());
        }
        else if(second_value.get_type()==turn)
            max_turn_number = std::max(max_turn_number,int(second_value.get_value()));
    }
}

piece_placement_goal::piece_placement_goal(void)noexcept:piece(),x(0),y(0){
}
piece_placement_goal::piece_placement_goal(token&& p,uint x_coord,uint y_coord):piece(std::move(p)),x(x_coord),y(y_coord){
}

parser_result<piece_placement_goal> parse_piece_placement_goal(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
    uint board_height,
    uint board_width,
    messages_container& msg)throw(message){
    if(!it.has_value()||it.current().get_type()!=at_sign)
        return failure<piece_placement_goal>();
    if(!it.next(msg))
        throw msg.build_message("Unexpected end of goal");
    if(it.current().get_type()!=identifier)
        throw msg.build_message(it.create_call_stack("Expected identifier, encountered \'"+it.current().to_string()+"\'"));
    if(encountered_pieces.count(it.current())==0)
        throw msg.build_message(it.create_call_stack("There's no piece \'"+it.current().to_string()+"\'"));
    token piece = it.current();
    if(!it.next(msg))
        throw msg.build_message("Unexpected end of goal");
    slice_iterator before_x = it;
    parser_result<int> coord = parse_int(it,msg);
    if(!coord.is_success())
        throw msg.build_message(it.create_call_stack("Expected integer, encountered \'"+it.current().to_string()+"\'"));
    if(coord.get_value()<0||coord.get_value()>=int(board_width))
        throw msg.build_message(before_x.create_call_stack("Coord out of board"));
    uint x = coord.get_value();
    if(!it.has_value())
        throw msg.build_message("Unexpected end of goal");
    slice_iterator before_y = it;
    coord = parse_int(it,msg);
    if(!coord.is_success())
        throw msg.build_message(it.create_call_stack("Expected integer, encountered \'"+it.current().to_string()+"\'"));
    if(coord.get_value()<0||coord.get_value()>=int(board_height))
        throw msg.build_message(before_y.create_call_stack("Coord out of board"));
    uint y = coord.get_value();
    return success(piece_placement_goal(std::move(piece),x,y));
}

bool piece_placement_goal::operator==(const piece_placement_goal& m)const{
    return piece==m.piece&&x==m.x&&y==m.y;
}

bool piece_placement_goal::operator<(const piece_placement_goal& m)const{
    return piece<m.piece
        || (piece==m.piece&&x<m.x)
        || (piece==m.piece&&x==m.x&&y<m.y);
}

std::ostream& operator<<(std::ostream& out,const piece_placement_goal& g){
    return out<<'@'<<g.piece.to_string()<<' '<<g.x<<' '<<g.y;
}

negatable_goal::negatable_goal(void)noexcept:
negated(false),
alternative(nullptr),
tag(0){}

negatable_goal::negatable_goal(const negatable_goal& src)noexcept:
negated(src.negated),
alternative(nullptr),
tag(src.tag){
    switch(src.tag){
    case 0:
        alternative = new goals_alternative(*src.alternative);
        break;
    case 1:
        atomic = new atomic_goal(*src.atomic);
        break;
    case 2:
        move_goal = new moves_sum(*src.move_goal);
        break;
    default:
        piece_placement = new piece_placement_goal(*src.piece_placement);
    }
}

negatable_goal& negatable_goal::operator=(const negatable_goal& src)noexcept{
    if(this != &src){
        negated = src.negated;
        switch(tag){
        case 0:
            delete alternative;
            break;
        case 1:
            delete atomic;
            break;
        case 2:
            delete move_goal;
            break;
        default:
            delete piece_placement;
        }
        tag = src.tag;
        switch(src.tag){
        case 0:
            alternative = new goals_alternative(*src.alternative);
            break;
        case 1:
            atomic = new atomic_goal(*src.atomic);
            break;
        case 2:
            move_goal = new moves_sum(*src.move_goal);
            break;
        default:
            piece_placement = new piece_placement_goal(*src.piece_placement);
        }
    }
    return *this;
}

negatable_goal::negatable_goal(negatable_goal&& src)noexcept:
negated(src.negated),
alternative(),
tag(src.tag){
    switch(src.tag){
    case 0:
        alternative = src.alternative;
        src.alternative = nullptr;
        break;
    case 1:
        atomic = src.atomic;
        src.atomic = nullptr;
        break;
    case 2:
        move_goal = src.move_goal;
        src.move_goal = nullptr;
        break;
    default:
        piece_placement = src.piece_placement;
        src.piece_placement = nullptr;
        break;
    }
}

negatable_goal& negatable_goal::operator=(negatable_goal&& src)noexcept{
    if(this != &src){
        negated = src.negated;
        switch(tag){
        case 0:
            delete alternative;
            break;
        case 1:
            delete atomic;
            break;
        case 2:
            delete move_goal;
            break;
        default:
            delete piece_placement;
        }
        tag = src.tag;
        switch(src.tag){
        case 0:
            alternative = src.alternative;
            break;
        case 1:
            atomic = src.atomic;
            break;
        case 2:
            move_goal = src.move_goal;
            break;
        default:
            piece_placement = src.piece_placement;
        }
        src.tag = 0;
        src.alternative = nullptr;
    }
    return *this;
}

negatable_goal::negatable_goal(goals_alternative&& src)noexcept:
negated(0),
tag(0){
    alternative = new goals_alternative(std::move(src));
}

negatable_goal::negatable_goal(atomic_goal&& src)noexcept:
negated(0),
tag(1){
    atomic = new atomic_goal(std::move(src));
}

negatable_goal::negatable_goal(moves_sum&& src)noexcept:
negated(0),
tag(2){
    move_goal = new moves_sum(std::move(src));
}

negatable_goal::negatable_goal(piece_placement_goal&& src)noexcept:
negated(0),
tag(3){
    piece_placement = new piece_placement_goal(std::move(src));
}

negatable_goal::~negatable_goal(void)noexcept{
    switch(tag){
    case 0:
        delete alternative;
        break;
    case 1:
        delete atomic;
        break;
    case 2:
        delete move_goal;
        break;
    default:
        delete piece_placement;
    }
}

parser_result<negatable_goal> parse_negatable_goal(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
    uint board_height,
    uint board_width,
    messages_container& msg)throw(message){
    if(!it.has_value())
        return failure<negatable_goal>();
    bool fatal = false;
    bool negated = false;
    negatable_goal result;
    if(it.current().get_type()==logical_not){
        negated = true;
        fatal = true;
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of goal");
    }
    if(it.current().get_type()==left_round_bracket){
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of goal");
        parser_result<goals_alternative> g_result = parse_goals_alternative(it,encountered_pieces,board_height,board_width,msg);
        if(!g_result.is_success())
            throw msg.build_message(it.create_call_stack("Expected goals alternative, encountered \'"+it.current().to_string()+"\'"));
        if(!it.has_value())
            throw msg.build_message("Unexpected end of goal");
        if(it.current().get_type()!=right_round_bracket)
            throw msg.build_message(it.create_call_stack("Expected \')\', encountered \'"+it.current().to_string()+"\'"));
        it.next(msg);
        result = g_result.move_value();
    }
    else if(it.current().get_type()==move){
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of goal");
        if(it.current().get_type()!=left_round_bracket)
            throw msg.build_message(it.create_call_stack("Expected \'(\', encountered \'"+it.current().to_string()+"\'"));
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of goal");
        slice_iterator before_m(it);
        std::set<token> dummy_set;
        game_order dummy_players_set;
        bool dummy_bool;
        parser_result<moves_sum> g_result = parse_moves_sum(it,dummy_set,dummy_players_set,0,dummy_bool,msg);
        if(!g_result.is_success())
            throw msg.build_message(it.create_call_stack("Expected move pattern, encountered \'"+it.current().to_string()+"\'"));
        if(!g_result.get_value().is_goal_eligible())
            throw msg.build_message(before_m.create_call_stack("Move patterns in goals cannot contain offs nor turn changers"));
        if(!it.has_value())
            throw msg.build_message("Unexpected end of goal");
        if(it.current().get_type()!=right_round_bracket)
            throw msg.build_message(it.create_call_stack("Expected \')\', encountered \'"+it.current().to_string()+"\'"));
        it.next(msg);
        result = g_result.move_value();
    }
    else{
        parser_result<piece_placement_goal> p_result = parse_piece_placement_goal(it,encountered_pieces,board_height,board_width,msg);
        if(p_result.is_success())
            result = p_result.move_value();
        else{
            parser_result<atomic_goal> g_result = parse_atomic_goal(it,encountered_pieces,msg);
            if(g_result.is_success())
                result = g_result.move_value();
            else{
                if(fatal)
                    throw msg.build_message(it.create_call_stack("Expected goals alternative, atomic goal or \'move\' token, encountered \'"+it.current().to_string()+"\'"));
                else
                    return failure<negatable_goal>();
            }
        }
    }
    if(negated)
        result.negate();
    return success(std::move(result));
}

bool negatable_goal::operator<(const negatable_goal& m)const{
    if(tag<m.tag)
        return true;
    else if(tag>m.tag)
        return false;
    else if(negated<m.negated)
        return true;
    else if(negated>m.negated)
        return false;
    switch(tag){
    case 0:
        return *alternative<*m.alternative;
    case 1:
        return *atomic<*m.atomic;
    case 2:
        return *move_goal<*m.move_goal;
    default:
        return *piece_placement<*piece_placement;
    }
}

bool negatable_goal::operator==(const negatable_goal& m)const{
    if(tag!=m.tag||negated!=m.negated)
        return false;
    switch(tag){
    case 0:
        return *alternative==*m.alternative;
    case 1:
        return *atomic==*m.atomic;
    case 2:
        return *move_goal==*m.move_goal;
    default:
        return *piece_placement==*piece_placement;
    }
}

void negatable_goal::negate(void){
    negated = !negated;
}

void negatable_goal::print_rbg(std::ostream& out,uint recurrence_depth)const{
    if(negated)
        out<<"not ";
    switch(tag){
    case 0:
        out<<"(\n";
        alternative->print_rbg(out,recurrence_depth+1);
        print_spaces(out,4*recurrence_depth);
        out<<')';
        break;
    case 1:
        out<<(*atomic);
        break;
    case 2:
        out<<"move(\n    ";
        move_goal->print_rbg(out,recurrence_depth+1);
        print_spaces(out,4*recurrence_depth);
        out<<')';
        break;
    default:
        out<<(*piece_placement);
    }
}

void negatable_goal::apply_negation(bool should_be_negated){
    if(negated&&should_be_negated){
        negated=false;
        should_be_negated=false;
    }
    else if(negated){
        negated=false;
        should_be_negated=true;
    }
    switch(tag){
        case 0:
            alternative->apply_negation(should_be_negated);
            break;
        case 1:
        case 2:
        default:
            negated = should_be_negated;
    }
}

bool negatable_goal::is_single_alternative(void)const{
    return tag==0;
}

goals_alternative negatable_goal::give_single_alternative(void){
    assert(is_single_alternative());
    goals_alternative result = std::move(*alternative);
    return result;
}

bool negatable_goal::is_single_conjunction(void)const{
    return tag==0&&alternative->is_single_conjunction();
}

goals_conjunction negatable_goal::give_single_conjunction(void){
    assert(is_single_conjunction());
    return alternative->give_single_conjunction();
}

negatable_goal negatable_goal::flatten(void){
    switch(tag){
        case 0:
            if(alternative->is_single_negation())
                return alternative->give_single_negation().flatten();
            else
                return alternative->flatten();
        case 1:
        case 2:
        default:
            return std::move(*this);
    }
}

void negatable_goal::gather_information(
    int& max_turn_number,
    std::map<token,std::set<int>>& possible_comparisons,
    std::set<token>& should_count,
    uint board_size)const{
    switch(tag){
        case 0:
            alternative->gather_information(max_turn_number,possible_comparisons,should_count,board_size);
            break;
        case 1:
            atomic->gather_information(max_turn_number,possible_comparisons,should_count,board_size);
            break;
        case 2:
        default:
            break;
    }
}

goals_conjunction::goals_conjunction(std::vector<negatable_goal>&& src)noexcept:
content(std::move(src)){}

goals_conjunction::goals_conjunction(negatable_goal&& src)noexcept:
content(){
    content.push_back(std::move(src));
}

goals_conjunction::goals_conjunction(void)noexcept:
content(){}

parser_result<goals_conjunction> parse_goals_conjunction(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
    uint board_height,
    uint board_width,
    messages_container& msg)throw(message){
    if(!it.has_value())
        return failure<goals_conjunction>();
    std::vector<negatable_goal> result;
    parser_result<negatable_goal> g_result = parse_negatable_goal(it,encountered_pieces,board_height,board_width,msg);
    if(g_result.is_success())
        result.push_back(g_result.move_value());
    else
        return failure<goals_conjunction>();
    while(it.has_value() && it.current().get_type() == logical_and){
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of goals conjunction");
        g_result = parse_negatable_goal(it,encountered_pieces,board_height,board_width,msg);
        if(!g_result.is_success())
            throw msg.build_message(it.create_call_stack("Expected goals alternative, atomic goal, move pattern, piece placement goal or their negation, encountered \'"+it.current().to_string()+"\'"));
        else
            result.push_back(g_result.move_value());
    }
    return success(goals_conjunction(std::move(result)));
}

bool goals_conjunction::operator<(const goals_conjunction& m)const{
    if(content.size()<m.content.size())
        return true;
    else if(content.size()>m.content.size())
        return false;
    else{
        for(auto i1=content.begin(), i2=m.content.begin();i1!=content.end();++i1,++i2){
            if(*i1 < *i2)
                return true;
            if(*i2 < *i1)
                return false;
        }
        return false;
    }
}

bool goals_conjunction::operator==(const goals_conjunction& m)const{
    if(content.size() != m.content.size())
        return false;
    else{
        for(auto i1=content.begin(), i2=m.content.begin();i1!=content.end();++i1,++i2)
            if(!(*i1 == *i2))
                return false;
        return true;
    }
}

void goals_conjunction::print_rbg(std::ostream& out,uint recurrence_depth)const{
    if(!content.empty()){
        auto it = content.begin();
        it->print_rbg(out,recurrence_depth);
        while((++it)!=content.end()){
            out<<" and ";
            it->print_rbg(out,recurrence_depth);
        }
    }
}

void goals_conjunction::apply_negation(bool should_be_negated){
    std::vector<goals_conjunction> possible_result;
    for(uint i=0;i<content.size();++i){
        content[i].apply_negation(should_be_negated);
        if(should_be_negated)
            possible_result.push_back(goals_conjunction(std::move(content[i])));
    }
    if(should_be_negated){
        content.clear();
        content.push_back(goals_alternative(std::move(possible_result)));
    }
}

bool goals_conjunction::is_single_negation(void)const{
    return content.size()==1;
}

negatable_goal goals_conjunction::give_single_negation(void){
    assert(is_single_negation());
    negatable_goal result = std::move(content.back());
    content.clear();
    return result;
}

bool goals_conjunction::is_single_alternative(void)const{
    return content.size()==1&&content.back().is_single_alternative();
}

goals_alternative goals_conjunction::give_single_alternative(void){
    assert(is_single_alternative());
    goals_alternative result = content.back().give_single_alternative();
    content.clear();
    return result;
}

goals_conjunction goals_conjunction::flatten(void){
    std::vector<std::pair<std::vector<negatable_goal>::iterator,std::vector<negatable_goal>>> goals_stack;
    goals_stack.push_back(make_pair(content.begin(),std::move(content)));
    goals_stack.back().first = goals_stack.back().second.begin(); // inelegant way to avoid undefined behavior
    content.clear();
    std::vector<negatable_goal> result;
    while(!goals_stack.empty()){
        auto it = goals_stack.back().first;
        if(it == goals_stack.back().second.end())
            goals_stack.pop_back();
        else if(it->is_single_conjunction()){
            goals_conjunction next_level = it->give_single_conjunction();
            ++goals_stack.back().first;
            goals_stack.push_back(make_pair(next_level.content.begin(),std::move(next_level.content)));
            goals_stack.back().first = goals_stack.back().second.begin(); // inelegant way to avoid undefined behavior
        }
        else{
            result.push_back(goals_stack.back().first->flatten());
            ++goals_stack.back().first;
        }
    }
    return goals_conjunction(std::move(result));
}

void goals_conjunction::gather_information(
    int& max_turn_number,
    std::map<token,std::set<int>>& possible_comparisons,
    std::set<token>& should_count,
    uint board_size)const{
    for(uint i=0;i<content.size();++i)
        content[i].gather_information(max_turn_number,possible_comparisons,should_count,board_size);
}

goals_alternative::goals_alternative(std::vector<goals_conjunction>&& src)noexcept:
content(std::move(src)){}

goals_alternative::goals_alternative(goals_conjunction&& src)noexcept:
content(){
    content.push_back(std::move(src));
}

goals_alternative::goals_alternative(void)noexcept:
content(){}

parser_result<goals_alternative> parse_goals_alternative(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
    uint board_height,
    uint board_width,
    messages_container& msg,
    bool can_be_empty)throw(message){
    std::vector<goals_conjunction> result;
    parser_result<goals_conjunction> g_result = parse_goals_conjunction(it,encountered_pieces,board_height,board_width,msg);
    if(g_result.is_success())
        result.push_back(g_result.move_value());
    else
        return (can_be_empty ? success(goals_alternative(std::move(result))) : failure<goals_alternative>());
    while(it.has_value() && it.current().get_type() == logical_or){
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of goals alternative");
        g_result = parse_goals_conjunction(it,encountered_pieces,board_height,board_width,msg);
        if(!g_result.is_success())
            throw msg.build_message(it.create_call_stack("Expected goals conjunction, encountered \'"+it.current().to_string()+"\'"));
        else
            result.push_back(g_result.move_value());
    }
    return success(goals_alternative(std::move(result)));
}

bool goals_alternative::operator<(const goals_alternative& m)const{
    if(content.size()<m.content.size())
        return true;
    else if(content.size()>m.content.size())
        return false;
    else{
        for(auto i1=content.begin(), i2=m.content.begin();i1!=content.end();++i1,++i2){
            if(*i1 < *i2)
                return true;
            if(*i2 < *i1)
                return false;
        }
        return false;
    }
}

bool goals_alternative::operator==(const goals_alternative& m)const{
    if(content.size() != m.content.size())
        return false;
    else{
        for(auto i1=content.begin(), i2=m.content.begin();i1!=content.end();++i1,++i2)
            if(!(*i1 == *i2))
                return false;
        return true;
    }
}

void goals_alternative::print_rbg(std::ostream& out,uint recurrence_depth)const{
    if(!content.empty()){
        auto it = content.begin();
        print_spaces(out,4*recurrence_depth);
        it->print_rbg(out,recurrence_depth);
        out<<'\n';
        while((++it)!=content.end()){
            print_spaces(out,4*(recurrence_depth-1));
            out<<" or ";
            it->print_rbg(out,recurrence_depth);
            out<<'\n';
        }
    }
}

std::ostream& operator<<(std::ostream& out,const goals_alternative& g){
    g.print_rbg(out,1);
    return out;
}

void goals_alternative::apply_negation(bool should_be_negated){
    std::vector<negatable_goal> possible_result;
    for(uint i=0;i<content.size();++i){
        content[i].apply_negation(should_be_negated);
        if(should_be_negated)
            possible_result.push_back(negatable_goal(std::move(content[i])));
    }
    if(should_be_negated){
        content.clear();
        content.push_back(std::move(possible_result));
    }
}

bool goals_alternative::is_single_conjunction(void)const{
    return content.size()==1;
}

goals_conjunction goals_alternative::give_single_conjunction(void){
    assert(is_single_conjunction());
    goals_conjunction result = std::move(content.back());
    content.clear();
    return result;
}

bool goals_alternative::is_single_negation(void)const{
    return content.size()==1&&content.back().is_single_negation();
}

negatable_goal goals_alternative::give_single_negation(void){
    assert(is_single_negation());
    negatable_goal result = content.back().give_single_negation();
    content.clear();
    return result;
}

goals_alternative goals_alternative::flatten(void){
    std::vector<std::pair<std::vector<goals_conjunction>::iterator,std::vector<goals_conjunction>>> goals_stack;
    goals_stack.push_back(make_pair(content.begin(),std::move(content)));
    goals_stack.back().first = goals_stack.back().second.begin(); // inelegant way to avoid undefined behavior
    content.clear();
    std::vector<goals_conjunction> result;
    while(!goals_stack.empty()){
        auto it = goals_stack.back().first;
        if(it == goals_stack.back().second.end())
            goals_stack.pop_back();
        else if(it->is_single_alternative()){
            goals_alternative next_level = it->give_single_alternative();
            ++goals_stack.back().first;
            goals_stack.push_back(make_pair(next_level.content.begin(),std::move(next_level.content)));
            goals_stack.back().first = goals_stack.back().second.begin(); // inelegant way to avoid undefined behavior
        }
        else{
            result.push_back(goals_stack.back().first->flatten());
            ++goals_stack.back().first;
        }
    }
    return goals_alternative(std::move(result));
}

void goals_alternative::gather_information(
    int& max_turn_number,
    std::map<token,std::set<int>>& possible_comparisons,
    std::set<token>& should_count,
    uint board_size)const{
    for(uint i=0;i<content.size();++i)
        content[i].gather_information(max_turn_number,possible_comparisons,should_count,board_size);
}

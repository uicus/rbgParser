#include"game_goal.hpp"

atomic_goal::atomic_goal(void):
compared_value(), // dummy
compared_const(0),
kind_of_comparison(0){}

atomic_goal::atomic_goal(token&& t,uint n,uint kind):
compared_value(std::move(t)),
compared_const(n),
kind_of_comparison(kind){}

parser_result<atomic_goal> parse_atomic_goal(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
    messages_container& msg)throw(message){
    if(!it.has_value())
        return failure<atomic_goal>();
    parser_result<token> variable = parse_variable(it,encountered_pieces,msg);
    uint kind;
    if(variable.is_success()){
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
        slice_iterator before_int(it);
        parser_result<int> constant = parse_int(it,msg);
        if(constant.is_success()){
            if(constant.get_value()<0)
                throw msg.build_message(before_int.create_call_stack("Constants in goal must be non-negative"));
            return success(atomic_goal(variable.move_value(),constant.get_value(),kind));
        }
        else
            throw msg.build_message(before_int.create_call_stack("Expected non-negative integer, enountered \'"+before_int.current().to_string()+"\'"));
    }
    else{
        slice_iterator before_int(it);
        parser_result<int> constant = parse_int(it,msg);
        if(constant.is_success()){
            if(constant.get_value()<0)
                throw msg.build_message(before_int.create_call_stack("Constants in goal must be non-negative"));
            if(!it.has_value())
                throw msg.build_message("Unexpected end of goal");
            if(it.current().get_type()==less)
                kind = 4;
            else if(it.current().get_type()==less_equal)
                kind = 3;
            else if(it.current().get_type()==equal)
                kind = 2;
            else if(it.current().get_type()==greater_equal)
                kind = 1;
            else if(it.current().get_type()==greater)
                kind = 0;
            else
                throw msg.build_message(it.create_call_stack("Expected \'<\', \'<=\', \'=\', \'>=\' or \'>\', encountered \'"+it.current().to_string()+"\'"));
            if(!it.next(msg))
                throw msg.build_message("Unexpected end of goal");
            variable = parse_variable(it,encountered_pieces,msg);
            if(variable.is_success())
                return success(atomic_goal(variable.move_value(),constant.get_value(),kind));
            else
                throw msg.build_message(it.create_call_stack("Expected \'$\' preceded piece name or \'turn\' string, encountered \'"+it.current().to_string()+"\'"));
        }
        else
            return failure<atomic_goal>();
    }
}

bool atomic_goal::operator==(const atomic_goal& m)const{
    return compared_value==m.compared_value && compared_const==m.compared_const && kind_of_comparison==m.kind_of_comparison;
}

bool atomic_goal::operator<(const atomic_goal& m)const{
    return compared_value<m.compared_value
        || (compared_value==m.compared_value && compared_const<m.compared_const)
        || (compared_value==m.compared_value && compared_const==m.compared_const && kind_of_comparison<m.kind_of_comparison);
}

std::ostream& operator<<(std::ostream& out,const atomic_goal& g){
    out<<'$'<<g.compared_value.to_string();
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
    out<<g.compared_const;
    return out;
}

negatable_goal::negatable_goal(void):
negated(false),
alternative(nullptr),
tag(0){}

negatable_goal::negatable_goal(const negatable_goal& src):
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
    default:
        move_goal = new moves_sum(*src.move_goal);
    }
}

negatable_goal& negatable_goal::operator=(const negatable_goal& src){
    if(this != &src){
        negated = src.negated;
        switch(tag){
        case 0:
            delete alternative;
            break;
        case 1:
            delete atomic;
            break;
        default:
            delete move_goal;
        }
        tag = src.tag;
        switch(src.tag){
        case 0:
            alternative = new goals_alternative(*src.alternative);
            break;
        case 1:
            atomic = new atomic_goal(*src.atomic);
            break;
        default:
            move_goal = new moves_sum(*src.move_goal);
        }
    }
    return *this;
}

negatable_goal::negatable_goal(negatable_goal&& src):
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
    default:
        move_goal = src.move_goal;
        src.move_goal = nullptr;
    }
}

negatable_goal& negatable_goal::operator=(negatable_goal&& src){
    if(this != &src){
        negated = src.negated;
        switch(tag){
        case 0:
            delete alternative;
            break;
        case 1:
            delete atomic;
            break;
        default:
            delete move_goal;
        }
        tag = src.tag;
        switch(src.tag){
        case 0:
            alternative = src.alternative;
            break;
        case 1:
            atomic = src.atomic;
            break;
        default:
            move_goal = src.move_goal;
        }
        src.tag = 0;
        src.alternative = nullptr;
    }
    return *this;
}

negatable_goal::negatable_goal(goals_alternative&& src):
negated(0),
tag(0){
    alternative = new goals_alternative(std::move(src));
}

negatable_goal::negatable_goal(atomic_goal&& src):
negated(0),
tag(1){
    atomic = new atomic_goal(std::move(src));
}

negatable_goal::negatable_goal(moves_sum&& src):
negated(0),
tag(2){
    move_goal = new moves_sum(std::move(src));
}

negatable_goal::~negatable_goal(void){
    switch(tag){
    case 0:
        delete alternative;
        break;
    case 1:
        delete atomic;
        break;
    default:
        delete move_goal;
    }
}

parser_result<negatable_goal> parse_negatable_goal(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
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
        parser_result<goals_alternative> g_result = parse_goals_alternative(it,encountered_pieces,msg);
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
    default:
        return *move_goal<*m.move_goal;
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
    default:
        return *move_goal==*m.move_goal;
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
    default:
        out<<"move(\n    ";
        move_goal->print_rbg(out,recurrence_depth+1);
        print_spaces(out,4*recurrence_depth);
        out<<')';
    }
}

goals_conjunction::goals_conjunction(std::set<negatable_goal>&& src):
content(std::move(src)){}

goals_conjunction::goals_conjunction(void):
content(){}

parser_result<goals_conjunction> parse_goals_conjunction(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
    messages_container& msg)throw(message){
    if(!it.has_value())
        return failure<goals_conjunction>();
    std::set<negatable_goal> result;
    parser_result<negatable_goal> g_result = parse_negatable_goal(it,encountered_pieces,msg);
    if(g_result.is_success())
        result.insert(g_result.move_value());
    else
        return failure<goals_conjunction>();
    while(it.has_value() && it.current().get_type() == logical_and){
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of goals conjunction");
        g_result = parse_negatable_goal(it,encountered_pieces,msg);
        if(!g_result.is_success())
            throw msg.build_message(it.create_call_stack("Expected goals alternative, atomic goal, move pattern or their negation, encountered \'"+it.current().to_string()+"\'"));
        else
            result.insert(g_result.move_value());
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

goals_alternative::goals_alternative(std::set<goals_conjunction>&& src):
content(std::move(src)){}

goals_alternative::goals_alternative(void):
content(){}

parser_result<goals_alternative> parse_goals_alternative(
    slice_iterator& it,
    const std::set<token>& encountered_pieces,
    messages_container& msg,
    bool can_be_empty)throw(message){
    std::set<goals_conjunction> result;
    parser_result<goals_conjunction> g_result = parse_goals_conjunction(it,encountered_pieces,msg);
    if(g_result.is_success())
        result.insert(g_result.move_value());
    else
        return (can_be_empty ? success(goals_alternative(std::move(result))) : failure<goals_alternative>());
    while(it.has_value() && it.current().get_type() == logical_or){
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of goals alternative");
        g_result = parse_goals_conjunction(it,encountered_pieces,msg);
        if(!g_result.is_success())
            throw msg.build_message(it.create_call_stack("Expected goals conjunction, encountered \'"+it.current().to_string()+"\'"));
        else
            result.insert(g_result.move_value());
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

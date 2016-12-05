#include"game_move.hpp"

atomic_move::atomic_move(void):
x(0),
y(0),
on(),
off(),
every_on_legal(false),
no_off(false){}

atomic_move::atomic_move(int _x,int _y,const std::set<token>& _on,const std::set<token>& _off):
x(_x),
y(_y),
on(_on),
off(_off),
every_on_legal(false),
no_off(false){}

atomic_move::atomic_move(int _x,int _y,const std::set<token>& _on_off, bool on_switch):
x(_x),
y(_y),
on(on_switch ? _on_off : std::set<token>()),
off(on_switch ? std::set<token>() : _on_off),
every_on_legal(!on_switch),
no_off(on_switch){}

parser_result<atomic_move> parse_atomic_move(
    slice_iterator& it,
    std::set<token>& encountered_pieces,
    messages_container& msg)throw(message){
    if(!it.has_value())
        return failure<atomic_move>();
    if(!it.current().get_type() != number && !it.current().get_type() != slash && !it.current().get_type() != identifier)
        return failure<atomic_move>();
    int x=0,y=0;
    parser_result<int> delta = parse_int(it,msg);
    if(delta.is_success()){
        x = delta.get_value();
        if(!it.has_value())
            throw msg.build_message("Unexpected end of atomic move");
        if(it.current().get_type() != comma)
            throw msg.build_message(it.create_call_stack("Expected ',', encountered \'"+it.current().to_string()+"\'"));
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of atomic move");
        delta = parse_int(it,msg);
        if(delta.is_success())
            y = delta.get_value();
        else
            throw msg.build_message(it.create_call_stack("Expected integer, encountered \'"+it.current().to_string()+"\'"));
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of atomic move");
        if(!it.current().get_type() != comma)
            throw msg.build_message(it.create_call_stack("Expected ',', encountered \'"+it.current().to_string()+"\'"));
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of atomic move");
    }
    bool every_on_legal = false;
    bool no_off = false;
    std::set<token> on,off;
    if(it.current().get_type() == slash){
        every_on_legal = true;
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of atomic move");
        parser_result<std::set<token>> legals = parse_tokens_set(it,msg);
        if(!legals.is_success())
            off = legals.get_value();
        else
            throw msg.build_message(it.create_call_stack("Expected set of identifiers, encountered \'"+it.current().to_string()+"\'"));
    }
    else{
        parser_result<std::set<token>> legals = parse_tokens_set(it,msg);
        if(legals.is_success())
            on = legals.get_value();
        else
            throw msg.build_message(it.create_call_stack("Expected set of identifiers or '/', encountered \'"+it.current().to_string()+"\'"));
        if(it.has_value()&&it.current().get_type() == slash){
            if(!it.next(msg))
                throw msg.build_message("Unexpected end of atomic move");
            legals = parse_tokens_set(it,msg);
            if(!legals.is_success())
                off = legals.get_value();
            else
                throw msg.build_message(it.create_call_stack("Expected set of identifiers, encountered \'"+it.current().to_string()+"\'"));
        }
        else
            no_off = true;
    }
    encountered_pieces.insert(on.begin(),on.end());
    encountered_pieces.insert(off.begin(),off.end());
    return success(every_on_legal ? atomic_move(x,y,off,false) : (no_off ? atomic_move(x,y,on,true) : atomic_move(x,y,on,off)));
}

bool atomic_move::operator<(const atomic_move& m)const{
    return x<m.x
        || (x==m.x && y<m.y)
        || (x==m.x && y==m.y && every_on_legal<m.every_on_legal)
        || (x==m.x && y==m.y && every_on_legal==m.every_on_legal && no_off<m.no_off)
        || (x==m.x && y==m.y && every_on_legal==m.every_on_legal && no_off==m.no_off && on<m.on)
        || (x==m.x && y==m.y && every_on_legal==m.every_on_legal && no_off==m.no_off && on==m.on && off<m.off);
}

bool atomic_move::operator==(const atomic_move& m)const{
    return (x==m.x && y==m.y && every_on_legal==m.every_on_legal && no_off==m.no_off && on==m.on && off==m.off);
}

turn_change_indicator::turn_change_indicator(const token& name):
player(name){
}

turn_change_indicator::turn_change_indicator(void):
player(create_quotation(0)){}//dummy

parser_result<turn_change_indicator> parse_turn_change_indicator(
    slice_iterator& it,
    const game_order& players,
    messages_container& msg)throw(message){
    if(!it.has_value())
        return failure<turn_change_indicator>();
    if(!it.current().get_type() != left_square_bracket)
        return failure<turn_change_indicator>();
    if(!it.next(msg))
        throw msg.build_message("Unexpected end of square brackets enclosed player name");
    if(it.current().get_type() != identifier)
        throw msg.build_message(it.create_call_stack("Expected player name, encountered \'"+it.current().to_string()+"\'"));
    if(!players.exists(it.current()))
        throw msg.build_message(it.create_call_stack("There is no player \'"+it.current().to_string()+"\'"));
    turn_change_indicator result(it.current());
    if(!it.next(msg))
        throw msg.build_message("Unexpected end of square brackets enclosed player name");
    if(it.current().get_type() != right_square_bracket)
        throw msg.build_message(it.create_call_stack("Expected \']\', encountered \'"+it.current().to_string()+"\'"));
    it.next(msg);
    return success<turn_change_indicator>(std::move(result));
}

bool turn_change_indicator::operator<(const turn_change_indicator& m)const{
    return player<m.player;
}

bool turn_change_indicator::operator==(const turn_change_indicator& m)const{
    return player==m.player;
}

bracketed_move::bracketed_move(void):
sum(nullptr),
tag(0){}

bracketed_move::bracketed_move(const bracketed_move& src):
sum(nullptr),
tag(src.tag){
    switch(src.tag){
    case 0:
        sum = new moves_sum(*src.sum);
        break;
    case 1:
        atomic = new atomic_move(*src.atomic);
        break;
    default:
        turn_changer = new turn_change_indicator(*src.turn_changer);
    }
}

bracketed_move& bracketed_move::operator=(const bracketed_move& src){
    if(this != &src){
        switch(tag){
        case 0:
            delete sum;
            break;
        case 1:
            delete atomic;
            break;
        default:
            delete turn_changer;
        }
        tag = src.tag;
        switch(src.tag){
        case 0:
            sum = new moves_sum(*src.sum);
            break;
        case 1:
            atomic = new atomic_move(*src.atomic);
            break;
        default:
            turn_changer = new turn_change_indicator(*src.turn_changer);
        }
    }
    return *this;
}

bracketed_move::bracketed_move(bracketed_move&& src):
sum(),
tag(src.tag){
    switch(src.tag){
    case 0:
        sum = src.sum;
        src.sum = nullptr;
        break;
    case 1:
        atomic = src.atomic;
        src.atomic = nullptr;
        break;
    default:
        turn_changer = src.turn_changer;
        src.turn_changer = nullptr;
    }
}

bracketed_move& bracketed_move::operator=(bracketed_move&& src){
    if(this != &src){
        switch(tag){
        case 0:
            delete sum;
            break;
        case 1:
            delete atomic;
            break;
        default:
            delete turn_changer;
        }
        tag = src.tag;
        switch(src.tag){
        case 0:
            sum = src.sum;
            break;
        case 1:
            atomic = src.atomic;
            break;
        default:
            turn_changer = src.turn_changer;
        }
        src.tag = 0;
        src.sum = nullptr;
    }
    return *this;
}

bracketed_move::bracketed_move(const moves_sum& src):
tag(0){
    sum = new moves_sum(src);
}

bracketed_move::bracketed_move(const atomic_move& src):
tag(1){
    atomic = new atomic_move(src);
}

bracketed_move::bracketed_move(const turn_change_indicator& src):
tag(2){
    turn_changer = new turn_change_indicator(src);
}

bracketed_move::~bracketed_move(void){
    switch(tag){
    case 0:
        delete sum;
        break;
    case 1:
        delete atomic;
        break;
    default:
        delete turn_changer;
    }
}

parser_result<bracketed_move> parse_bracketed_move(
    slice_iterator& it,
    std::set<token>& encountered_pieces,
    const game_order& players,
    messages_container& msg)throw(message){
    if(!it.has_value())
        return failure<bracketed_move>();
    parser_result<turn_change_indicator> turn_changer_result = parse_turn_change_indicator(it,players,msg);
    if(turn_changer_result.is_success())
        return success(bracketed_move(turn_changer_result.get_value()));
    else if(it.current().get_type() == left_square_bracket){
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of brackets enclosed move");
        bracketed_move result;
        parser_result<moves_sum> sum_result = parse_moves_sum(it,encountered_pieces,players,msg);
        if(sum_result.is_success())
            result = sum_result.get_value();
        else{
            parser_result<atomic_move> atomic_result = parse_atomic_move(it,encountered_pieces,msg);
            if(atomic_result.is_success())
                result = atomic_result.get_value();
            else
                throw msg.build_message(it.create_call_stack("Expected atomic move or moves sum"));
        }
        if(!it.has_value())
            throw msg.build_message("Unexpected end of brackets enclosed move");
        if(it.current().get_type() != right_round_bracket)
            throw msg.build_message(it.create_call_stack("Expected \')\', encountered \'"+it.current().to_string()+"\'"));
        it.next(msg);
        return success(std::move(result));
    }
    else
        return failure<bracketed_move>();
}

bool bracketed_move::operator<(const bracketed_move& m)const{
    if(tag<m.tag)
        return true;
    else if(tag>m.tag)
        return false;
    switch(tag){
    case 0:
        return *sum<*m.sum;
    case 1:
        return *atomic<*m.atomic;
    default:
        return *turn_changer<*m.turn_changer;
    }
}

bool bracketed_move::operator==(const bracketed_move& m)const{
    if(tag!=m.tag)
        return false;
    switch(tag){
    case 0:
        return *sum==*m.sum;
    case 1:
        return *atomic==*m.atomic;
    default:
        return *turn_changer==*m.turn_changer;
    }
}

moves_concatenation::moves_concatenation(void):
content(){}

moves_concatenation::moves_concatenation(std::vector<bracketed_move>&& src):
content(std::move(src)){}

parser_result<moves_concatenation> parse_moves_concatenation(
    slice_iterator& it,
    std::set<token>& encountered_pieces,
    const game_order& players,
    messages_container& msg)throw(message){
    if(!it.has_value())
        return failure<moves_concatenation>();
    std::vector<bracketed_move> result;
    parser_result<bracketed_move> brackets_result = parse_bracketed_move(it,encountered_pieces,players,msg);
    if(brackets_result.is_success())
        result.push_back(brackets_result.get_value());
    else
        return failure<moves_concatenation>();
    while(true){
        brackets_result = parse_bracketed_move(it,encountered_pieces,players,msg);
        if(!brackets_result.is_success())
            return success(moves_concatenation(std::move(result)));
        else
            result.push_back(brackets_result.get_value());
    }
}

bool moves_concatenation::operator<(const moves_concatenation& m)const{
    if(content.size()<m.content.size())
        return true;
    else if(content.size()>m.content.size())
        return false;
    else{
        for(uint i=0;i<content.size();++i){
            if(content[i]<m.content[i])
                return true;
            else if(m.content[i]<content[i])
                return false;
        }
        return false;
    }
}

bool moves_concatenation::operator==(const moves_concatenation& m)const{
    if(content.size()!=m.content.size())
        return false;
    else{
        for(uint i=0;i<content.size();++i)
            if(!(content[i]==m.content[i]))
                return false;
        return true;
    }
}

moves_sum::moves_sum(void):
content(){}

moves_sum::moves_sum(std::set<moves_concatenation>&& src):
content(std::move(src)){}

parser_result<moves_sum> parse_moves_sum(
    slice_iterator& it,
    std::set<token>& encountered_pieces,
    const game_order& players,
    messages_container& msg)throw(message){
    if(!it.has_value())
        return failure<moves_sum>();
    std::set<moves_concatenation> result;
    parser_result<moves_concatenation> concat_result = parse_moves_concatenation(it,encountered_pieces,players,msg);
    if(concat_result.is_success())
        result.insert(concat_result.get_value());
    else
        return failure<moves_sum>();
    while(it.has_value() && it.current().get_type() == plus){
        if(it.next(msg))
            throw msg.build_message("Unexpected end of move sum");
        concat_result = parse_moves_concatenation(it,encountered_pieces,players,msg);
        if(!concat_result.is_success())
            throw msg.build_message(it.create_call_stack("Expected moves concatenation, encountered \'"+it.current().to_string()+"\'"));
        else
            result.insert(concat_result.get_value());
    }
    return success(moves_sum(std::move(result)));
}

bool moves_sum::operator<(const moves_sum& m)const{
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
bool moves_sum::operator==(const moves_sum& m)const{
    if(content.size() != m.content.size())
        return false;
    else{
        for(auto i1=content.begin(), i2=m.content.begin();i1!=content.end();++i1,++i2)
            if(!(*i1 == *i2))
                return false;
        return true;
    }
}

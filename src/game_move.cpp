#include<cassert>
#include<algorithm>

#include"game_move.hpp"

atomic_move::atomic_move(void)noexcept:
x(0),
y(0),
on(),
off(),
every_on_legal(false),
no_off(false){}

atomic_move::atomic_move(int _x,int _y,std::set<token>&& _on,std::set<token>&& _off):
x(_x),
y(_y),
on(std::move(_on)),
off(std::move(_off)),
every_on_legal(false),
no_off(false){}

atomic_move::atomic_move(int _x,int _y,std::set<token>&& _on_off, bool on_switch):
x(_x),
y(_y),
on(on_switch ? std::move(_on_off) : std::set<token>()),
off(on_switch ? std::set<token>() : std::move(_on_off)),
every_on_legal(!on_switch),
no_off(on_switch){}

atomic_move::atomic_move(int _x,int _y):
x(_x),
y(_y),
on(),
off(),
every_on_legal(true),
no_off(true){}

parser_result<atomic_move> parse_atomic_move(
    slice_iterator& it,
    std::set<token>& encountered_pieces,
    messages_container& msg)throw(message){
    if(!it.has_value())
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
        if(!it.has_value())
            throw msg.build_message("Unexpected end of atomic move");
        if(it.current().get_type() != comma)
            return success(atomic_move(x,y));
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of atomic move");
    }
    else if(it.current().get_type() != slash&&it.current().get_type() != identifier&&it.current().get_type() != left_curly_bracket)
        return failure<atomic_move>();
    bool every_on_legal = false;
    bool no_off = false;
    std::set<token> on,off;
    if(it.current().get_type() == slash){
        every_on_legal = true;
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of atomic move");
        parser_result<std::set<token>> legals = parse_tokens_set(it,msg);
        if(legals.is_success())
            off = legals.move_value();
        else
            throw msg.build_message(it.create_call_stack("Expected set of identifiers, encountered \'"+it.current().to_string()+"\'"));
    }
    else{
        parser_result<std::set<token>> legals = parse_tokens_set(it,msg);
        if(legals.is_success())
            on = legals.move_value();
        else
            throw msg.build_message(it.create_call_stack("Expected set of identifiers or '/', encountered \'"+it.current().to_string()+"\'"));
        if(it.has_value()&&it.current().get_type() == slash){
            if(!it.next(msg))
                throw msg.build_message("Unexpected end of atomic move");
            legals = parse_tokens_set(it,msg);
            if(legals.is_success())
                off = legals.move_value();
            else
                throw msg.build_message(it.create_call_stack("Expected set of identifiers, encountered \'"+it.current().to_string()+"\'"));
        }
        else
            no_off = true;
    }
    encountered_pieces.insert(on.begin(),on.end());
    encountered_pieces.insert(off.begin(),off.end());
    return success(
        every_on_legal ?
            atomic_move(x,y,std::move(off),false) :
            (no_off ?
                atomic_move(x,y,std::move(on),true) :
                atomic_move(x,y,std::move(on),std::move(off))));
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

bool atomic_move::is_goal_eligible(void)const{
    return no_off;
}

std::ostream& operator<<(std::ostream& out,const atomic_move& m){
    out<<m.x<<','<<m.y;
    if(!m.every_on_legal||!m.no_off)
        out<<',';
    if(!m.every_on_legal)
        out<<m.on;
    if(!m.no_off)
        out<<'/'<<m.off;
    return out;
}

std::ostream& operator<<(std::ostream& out,const std::set<token>& s){
    out<<'{';
    if(!s.empty()){
        auto it = s.begin();
        out<<it->to_string();
        while((++it)!=s.end())
            out<<','<<it->to_string();
    }
    out<<'}';
    return out;
}

atomic_move atomic_move::flatten(void){
    auto result = std::move(*this);
    on.clear();
    off.clear();
    return result;
}

bool atomic_move::can_absorb(const atomic_move& right_hand_side)const{
    return right_hand_side.x == 0
        && right_hand_side.y == 0
        && (   (no_off && right_hand_side.every_on_legal)
            || (!no_off && !right_hand_side.every_on_legal && off == right_hand_side.on));
}

void atomic_move::absorb(atomic_move&& right_hand_side){
    off = std::move(right_hand_side.off);
    no_off = right_hand_side.no_off;
}

bool atomic_move::can_be_absorbed(const atomic_move& left_hand_side)const{
    return left_hand_side.can_absorb(*this);
}

void atomic_move::be_absorbed(atomic_move&& left_hand_side){
    x = left_hand_side.x;
    y = left_hand_side.y;
    on = std::move(left_hand_side.on);
    every_on_legal = left_hand_side.every_on_legal;
}

bool atomic_move::has_off(void)const{
    return !no_off;
}

bool atomic_move::is_in_place(void)const{
    return x==0 && y==0;
}

std::pair<atomic_move,atomic_move> atomic_move::prepare_to_split(std::set<token>& known_pieces,std::set<token>& pieces_after_split,uint& current_id){
    token splitter;
    do{
        splitter = "splitter"+std::to_string(current_id);
        ++current_id;
    }while(known_pieces.count(splitter));
    known_pieces.insert(splitter);
    pieces_after_split.insert(splitter);
    std::set<token> splitter_singleton1,splitter_singleton2;
    splitter_singleton1.insert(splitter);
    splitter_singleton2.insert(splitter);
    return std::make_pair(
        (every_on_legal ? atomic_move(x,y,std::move(splitter_singleton1),false) : atomic_move(x,y,std::move(on),std::move(splitter_singleton1))),
        (no_off ? atomic_move(0,0,std::move(splitter_singleton2),true) : atomic_move(0,0,std::move(splitter_singleton2),std::move(off))));
}

void atomic_move::to_semisteps(
    moves_sum& N,
    moves_sum& B,
    moves_sum& T,
    moves_sum& BT,
    const std::set<token>& splitters){
    if(off.size()==1&&splitters.count(*off.begin())==1){
        N = empty_expression();
        B = empty_expression();
        T = single_letter(std::move(*this));
    }
    else if(on.size()==1&&splitters.count(*on.begin())==1){
        N = empty_expression();
        B = single_letter(std::move(*this));
        T = empty_expression();
    }
    else{
        N = single_letter(std::move(*this));
        B = empty_expression();
        T = empty_expression();
    }
    BT = empty_expression();
}

turn_change_indicator::turn_change_indicator(const token& name)noexcept:
player(name){
}

turn_change_indicator::turn_change_indicator(void)noexcept:
player(){}//dummy

parser_result<turn_change_indicator> parse_turn_change_indicator(
    slice_iterator& it,
    const game_order& players,
    int player_number,
    messages_container& msg)throw(message){
    if(!it.has_value())
        return failure<turn_change_indicator>();
    if(it.current().get_type() != left_square_bracket)
        return failure<turn_change_indicator>();
    if(!it.next(msg))
        throw msg.build_message("Unexpected end of square brackets enclosed player name (player \'"+players.get_player_name(player_number,0).to_string()+"\')");
    turn_change_indicator result;
    if(it.current().get_type() == identifier){
        if(!players.exists(it.current()))
            throw msg.build_message(it.create_call_stack("There is no player \'"+it.current().to_string()+"\'"));
        result = turn_change_indicator(it.current());
        it.next(msg);
    }
    else{
        parser_result<int> delta_result = parse_int(it,msg);
        if(delta_result.is_success())
            result = turn_change_indicator(players.get_player_name(player_number,delta_result.get_value()));
        else
            throw msg.build_message(it.create_call_stack("Expected player name or integer, encountered \'"+it.current().to_string()+"\'"));
    }
    if(!it.has_value())
        throw msg.build_message("Unexpected end of square brackets enclosed player name (player \'"+players.get_player_name(player_number,0).to_string()+"\')");
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

std::ostream& operator<<(std::ostream& out,const turn_change_indicator& m){
    out<<'['<<m.player.to_string()<<']';
    return out;
}

turn_change_indicator turn_change_indicator::flatten(void){
    auto result = std::move(*this);
    player = token();
    return result;
}

void turn_change_indicator::to_semisteps(
    moves_sum& N,
    moves_sum& B,
    moves_sum& T,
    moves_sum& BT){
    N = single_letter(std::move(*this));
    B = empty_expression();
    T = empty_expression();
    BT = empty_expression();
}

bracketed_move::bracketed_move(void)noexcept:
repetition_number(1),
sum(nullptr),
tag(0){}

bracketed_move::bracketed_move(const bracketed_move& src)noexcept:
repetition_number(src.repetition_number),
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

bracketed_move& bracketed_move::operator=(const bracketed_move& src)noexcept{
    if(this != &src){
        repetition_number = src.repetition_number;
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

bracketed_move::bracketed_move(bracketed_move&& src)noexcept:
repetition_number(src.repetition_number),
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

bracketed_move& bracketed_move::operator=(bracketed_move&& src)noexcept{
    if(this != &src){
        repetition_number = src.repetition_number;
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

bracketed_move::bracketed_move(moves_sum&& src,uint rn)noexcept:
repetition_number(rn),
tag(0){
    sum = new moves_sum(std::move(src));
}

bracketed_move::bracketed_move(atomic_move&& src,uint rn)noexcept:
repetition_number(rn),
tag(1){
    atomic = new atomic_move(std::move(src));
}

bracketed_move::bracketed_move(turn_change_indicator&& src,uint rn)noexcept:
repetition_number(rn),
tag(2){
    turn_changer = new turn_change_indicator(std::move(src));
}

bracketed_move::~bracketed_move(void)noexcept{
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
    int player_number,
    bool& contains_turn_changer,
    messages_container& msg)throw(message){
    if(!it.has_value())
        return failure<bracketed_move>();
    parser_result<turn_change_indicator> turn_changer_result = parse_turn_change_indicator(it,players,player_number,msg);
    if(turn_changer_result.is_success()){
        contains_turn_changer = true;
        return success(bracketed_move(turn_changer_result.move_value()));
    }
    else if(it.current().get_type() == left_round_bracket){
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of brackets enclosed move (player \'"+players.get_player_name(player_number,0).to_string()+"\')");
        bracketed_move result;
        bool sum_contains_turn_changer = false;
        parser_result<moves_sum> sum_result = parse_moves_sum(it,encountered_pieces,players,player_number,sum_contains_turn_changer,msg);
        if(sum_result.is_success())
            result = sum_result.move_value();
        else{
            parser_result<atomic_move> atomic_result = parse_atomic_move(it,encountered_pieces,msg);
            if(atomic_result.is_success())
                result = atomic_result.move_value();
            else
                throw msg.build_message(it.create_call_stack("Expected atomic move or moves sum, encountered \'"+it.current().to_string()+"\'"));
        }
        if(!it.has_value())
            throw msg.build_message("Unexpected end of brackets enclosed move");
        if(it.current().get_type() != right_round_bracket)
            throw msg.build_message(it.create_call_stack("Expected \')\', encountered \'"+it.current().to_string()+"\'"));
        if(it.next(msg)&&it.current().get_type()==caret){
            if(!it.next(msg))
                throw msg.build_message("Unexpected end of repetition indicator (player \'"+players.get_player_name(player_number,0).to_string()+"\')");
            slice_iterator fallback_it(it);
            if(it.current().get_type()==star){
                if(sum_contains_turn_changer)
                    throw msg.build_message(fallback_it.create_call_stack("Moves containing turn changer cannot be repeated more than once"));
                result.set_star();
                it.next(msg);
            }
            else{
                slice_iterator before_int_it(it);
                parser_result<int> rn_result = parse_int(it,msg);
                if(!rn_result.is_success())
                    throw msg.build_message(it.create_call_stack("Expected \'*\' or positive number, encountered \'"+it.current().to_string()+"\'"));
                if(rn_result.get_value()<=0)
                    throw msg.build_message(before_int_it.create_call_stack("Number of repetition must be positive"));
                if(rn_result.get_value()>1&&sum_contains_turn_changer)
                    throw msg.build_message(fallback_it.create_call_stack("Moves containing turn changer cannot be repeated more than once"));
                result.set_repetition_number(rn_result.get_value());
            }
        }
        contains_turn_changer = sum_contains_turn_changer;
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
    else if(repetition_number<m.repetition_number)
        return true;
    else if(repetition_number>m.repetition_number)
        return false;
    switch(tag){
    case 0:
        return sum==nullptr||(sum!=m.sum&&m.sum!=nullptr&&*sum<*m.sum);
    case 1:
        return atomic==nullptr||(atomic!=m.atomic&&m.atomic!=nullptr&&*atomic<*m.atomic);
    default:
        return turn_changer==nullptr||(turn_changer!=m.turn_changer&&m.turn_changer!=nullptr&&*turn_changer<*m.turn_changer);
    }
}

bool bracketed_move::operator==(const bracketed_move& m)const{
    if(tag!=m.tag||repetition_number!=m.repetition_number)
        return false;
    switch(tag){
    case 0:
        return sum==m.sum || (sum!=nullptr&&m.sum!=nullptr&&*sum==*m.sum);
    case 1:
        return atomic==m.atomic || (atomic!=nullptr&&m.atomic!=nullptr&&*atomic==*m.atomic);
    default:
        return turn_changer==m.turn_changer || (turn_changer!=nullptr&&m.turn_changer!=nullptr&&*turn_changer==*m.turn_changer);
    }
}

bool bracketed_move::is_goal_eligible(void)const{
    switch(tag){
    case 0:
        return sum->is_goal_eligible();
    case 1:
        return atomic->is_goal_eligible();
    default:
        return false;
    }
}

bool bracketed_move::is_atomic_move(void)const{
    return tag == 1;
}

const atomic_move& bracketed_move::show_atomic_move(void)const{
    assert(is_atomic_move());
    return *atomic;
}

atomic_move bracketed_move::give_atomic_move(void){
    assert(is_atomic_move());
    return std::move(*atomic);
}

void bracketed_move::set_repetition_number(uint rn){
    assert(rn>0);
    repetition_number = rn;
}

void bracketed_move::set_star(void){
    repetition_number = 0;
}

void bracketed_move::print_rbg(std::ostream& out,uint recurrence_depth)const{
    switch(tag){
    case 0:
        out<<"(\n    ";
        sum->print_rbg(out,recurrence_depth+1);
        print_spaces(out,4*recurrence_depth);
        out<<')';
        break;
    case 1:
        out<<"("<<(*atomic)<<')';
        break;
    default:
        out<<(*turn_changer);
    }
    if(repetition_number==0)
        out<<"^*";
    else if(repetition_number>1)
        out<<'^'<<repetition_number;
}

bracketed_move bracketed_move::flatten(void){
    switch(tag){
    case 0:
        if(sum->is_single_bracket(repetition_number)){
            bracketed_move lower_brackets = sum->give_single_bracket();
            lower_brackets.repetition_number = lower_brackets.repetition_number*repetition_number;
            return lower_brackets;
        }
        else{
            auto result = sum->flatten();
            //delete sum;
            //sum = nullptr;
            return bracketed_move(moves_sum(std::move(result)),repetition_number);
        }
    case 1:
        {auto result = atomic->flatten();
        //delete atomic;
        //atomic = nullptr;
        return bracketed_move(atomic_move(std::move(result)),repetition_number);}
    default:
        {auto result = turn_changer->flatten();
        //delete turn_changer;
        //turn_changer = nullptr;
        return bracketed_move(turn_change_indicator(std::move(result)),repetition_number);}
    }
}

bool bracketed_move::is_single_concatenation(void)const{
    if(repetition_number!=1||tag!=0||sum==nullptr)
        return false;
    return sum->is_single_concatenation();
}

moves_concatenation bracketed_move::give_single_concatenation(void){
    assert(is_single_concatenation());
    return sum->give_single_concatenation();
}

bool bracketed_move::is_similar_repetitionwise(uint repetition_number_above)const{
    return repetition_number_above==1
        || repetition_number==1
        || (repetition_number_above==0 && repetition_number==0)
        || (repetition_number_above>0 && repetition_number>0);
}

bool bracketed_move::is_single_sum(void)const{
    return repetition_number==1 && tag==0 && sum!=nullptr;
}

moves_sum bracketed_move::give_single_sum(void){
    assert(is_single_sum());
    moves_sum result = std::move(*sum);
    delete sum;
    sum = nullptr;
    return result;
}

bool bracketed_move::can_absorb(const atomic_move& right_hand_side)const{
    if(repetition_number!=1)
        return false;
    switch(tag){
    case 0:
        return sum->can_absorb(right_hand_side);
    case 1:
        return atomic->can_absorb(right_hand_side);
    default:
        return false;
    }
}

void bracketed_move::absorb(atomic_move&& right_hand_side){
    assert(repetition_number==1);
    switch(tag){
    case 0:
        sum->absorb(std::move(right_hand_side));
        break;
    case 1:
        atomic->absorb(std::move(right_hand_side));
        break;
    default:
        assert(false);
    }
}

bool bracketed_move::can_be_absorbed(const atomic_move& left_hand_side)const{
    if(repetition_number!=1)
        return false;
    switch(tag){
    case 0:
        return sum->can_be_absorbed(left_hand_side);
    case 1:
        return atomic->can_be_absorbed(left_hand_side);
    default:
        return false;
    }
}

void bracketed_move::be_absorbed(atomic_move&& left_hand_side){
    assert(repetition_number==1);
    switch(tag){
    case 0:
        sum->be_absorbed(std::move(left_hand_side));
        break;
    case 1:
        atomic->be_absorbed(std::move(left_hand_side));
        break;
    default:
        assert(false);
    }
}

std::vector<bracketed_move> bracketed_move::prepare_to_split(
    std::set<token>& known_pieces,
    std::set<token>& pieces_after_split,
    uint& current_id,
    bool is_beginning,
    bool& is_end){
    std::vector<bracketed_move> result;
    switch(tag){
    case 0:
        if(repetition_number<=1){
            if(repetition_number==0)is_end=false;
            moves_sum sum_result = sum->prepare_to_split(known_pieces,pieces_after_split,current_id,is_beginning&&repetition_number==1,is_end);
            result.push_back(bracketed_move(std::move(sum_result),repetition_number));
            return result;
        }
        else{
            for(uint i=repetition_number;i>0;--i){
                moves_sum temp = moves_sum(*sum).prepare_to_split(known_pieces,pieces_after_split,current_id,is_beginning&&i==1,is_end);
                if(temp.is_single_concatenation()){
                    std::vector<bracketed_move> lower_result = temp.give_single_concatenation().move_out();
                    for(uint j=lower_result.size();j>0;--j)
                        result.push_back(std::move(lower_result[j-1]));
                }
                else
                    result.push_back(std::move(temp));
            }
            std::reverse(result.begin(),result.end());
            return result;
        }
    case 1:
        if(atomic->has_off()){
            if(repetition_number==0){
                is_end=false;
                auto atomic_result = atomic->prepare_to_split(known_pieces,pieces_after_split,current_id);
                std::vector<bracketed_move> vectored_pair;
                vectored_pair.push_back(bracketed_move(std::move(atomic_result.first)));
                vectored_pair.push_back(bracketed_move(std::move(atomic_result.second)));
                std::vector<moves_concatenation> temp;
                temp.push_back(std::move(vectored_pair));
                result.push_back(bracketed_move(std::move(temp),0));
                return result;
            }
            else{
                for(uint i=repetition_number;i>0;--i){
                    if((is_beginning&&i==1&&atomic->is_in_place())||is_end)
                        result.push_back(i==1 ? std::move(*this) : atomic_move(*atomic));
                    else{
                        auto atomic_result = (i==1 ?
                            atomic->prepare_to_split(known_pieces,pieces_after_split,current_id) :
                            atomic_move(*atomic).prepare_to_split(known_pieces,pieces_after_split,current_id));
                        result.push_back(bracketed_move(std::move(atomic_result.second)));
                        result.push_back(bracketed_move(std::move(atomic_result.first)));
                    }
                    is_end=false;
                }
                std::reverse(result.begin(),result.end());
                return result;
            }
        }
        else{
            result.push_back(std::move(*this));
            is_end=false;
            return result;
        }
    default:
        result.push_back(std::move(*this));
        return result;
    }
}

void bracketed_move::to_semisteps(
    moves_sum& N,
    moves_sum& B,
    moves_sum& T,
    moves_sum& BT,
    const std::set<token>& splitters){
    assert(repetition_number<=1);
    if(repetition_number==0){
        moves_sum lower_N,lower_B,lower_T,lower_BT;
        switch(tag){
        case 0:
            sum->to_semisteps(lower_N,lower_B,lower_T,lower_BT,splitters);
            break;
        case 1:
            atomic->to_semisteps(lower_N,lower_B,lower_T,lower_BT,splitters);
            break;
        default:
            assert(false);
        }
        lower_N.set_star();
        N = lower_N;
        B = lower_B;
        B.concat_move(moves_sum(lower_N));
        T = lower_N;
        T.concat_move(moves_sum(lower_T));
        BT = std::move(lower_B);
        BT.concat_move(std::move(lower_N));
        BT.concat_move(std::move(lower_T));
        BT.add_move(std::move(lower_BT));
    }
    else{
        switch(tag){
        case 0:
            sum->to_semisteps(N,B,T,BT,splitters);
            break;
        case 1:
            atomic->to_semisteps(N,B,T,BT,splitters);
            break;
        default:
            turn_changer->to_semisteps(N,B,T,BT);
        }
    }
}

moves_concatenation::moves_concatenation(void)noexcept:
content(){}

moves_concatenation::moves_concatenation(std::vector<bracketed_move>&& src)noexcept:
content(std::move(src)){}

parser_result<moves_concatenation> parse_moves_concatenation(
    slice_iterator& it,
    std::set<token>& encountered_pieces,
    const game_order& players,
    int player_number,
    bool& contains_turn_changer,
    messages_container& msg)throw(message){
    if(!it.has_value())
        return failure<moves_concatenation>();
    std::vector<bracketed_move> result;
    bool brackets_contains_turn_changer = false;
    parser_result<bracketed_move> brackets_result = parse_bracketed_move(it,encountered_pieces,players,player_number,brackets_contains_turn_changer,msg);
    contains_turn_changer |= brackets_contains_turn_changer;
    if(brackets_result.is_success())
        result.push_back(brackets_result.move_value());
    else
        return failure<moves_concatenation>();
    while(true){
        brackets_contains_turn_changer = false;
        slice_iterator fallback_it(it);
        brackets_result = parse_bracketed_move(it,encountered_pieces,players,player_number,brackets_contains_turn_changer,msg);
        if(!brackets_result.is_success())
            return success(moves_concatenation(std::move(result)));
        else{
            if(contains_turn_changer)
                throw msg.build_message(fallback_it.create_call_stack("No move can appear after turn changer in concatenation"));
            result.push_back(brackets_result.move_value());
            contains_turn_changer |= brackets_contains_turn_changer;
        }
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

bool moves_concatenation::is_goal_eligible(void)const{
    for(const auto& el: content)
        if(!el.is_goal_eligible())
            return false;
    return true;
}

void moves_concatenation::print_rbg(std::ostream& out,uint recurrence_depth)const{
    if(is_epsilon())
        out<<"(0,0)";
    else{
        for(const auto& el: content)
            el.print_rbg(out,recurrence_depth);
    }
}

moves_concatenation moves_concatenation::flatten(void){
    std::vector<std::pair<std::vector<bracketed_move>::iterator,std::vector<bracketed_move>>> moves_stack;
    moves_stack.push_back(make_pair(content.begin(),std::move(content)));
    moves_stack.back().first = moves_stack.back().second.begin(); // inelegant way to avoid undefined behavior
    content.clear();
    std::vector<bracketed_move> result;
    while(!moves_stack.empty()){
        if(moves_stack.back().first==moves_stack.back().second.end())
            moves_stack.pop_back();
        else if(moves_stack.back().first->is_single_concatenation()){
            moves_concatenation next_level = moves_stack.back().first->give_single_concatenation();
            ++moves_stack.back().first;
            moves_stack.push_back(make_pair(next_level.content.begin(),std::move(next_level.content)));
            moves_stack.back().first = moves_stack.back().second.begin();
        }
        else{
            bracketed_move next_to_insert = moves_stack.back().first->flatten();
            if(result.empty())
                result.push_back(std::move(next_to_insert));
            else{
                if(result.back().is_atomic_move()&&next_to_insert.can_be_absorbed(result.back().show_atomic_move())){
                    next_to_insert.be_absorbed(result.back().give_atomic_move());
                    result.back() = std::move(next_to_insert);
                }
                else if(next_to_insert.is_atomic_move()&&result.back().can_absorb(next_to_insert.show_atomic_move()))
                    result.back().absorb(next_to_insert.give_atomic_move());
                else
                    result.push_back(std::move(next_to_insert));
            }
            ++moves_stack.back().first;
        }
    }
    return moves_concatenation(std::move(result));
}

bool moves_concatenation::is_single_sum(void)const{
    if(content.size()!=1)
        return false;
    return content[0].is_single_sum();
}
moves_sum moves_concatenation::give_single_sum(void){
    assert(is_single_sum());
    return content[0].give_single_sum();
}

bool moves_concatenation::is_single_bracket(uint repetition_number_above)const{
    return content.size()==1 && content.front().is_similar_repetitionwise(repetition_number_above);
}

bracketed_move moves_concatenation::give_single_bracket(void){
    return std::move(content.front());
}

bool moves_concatenation::can_absorb(const atomic_move& right_hand_side)const{
    if(content.empty())
        return false;
    return content.back().can_absorb(right_hand_side);
}

void moves_concatenation::absorb(atomic_move&& right_hand_side){
    content.back().absorb(std::move(right_hand_side));
}

bool moves_concatenation::can_be_absorbed(const atomic_move& left_hand_side)const{
    if(content.empty())
        return false;
    return content.front().can_be_absorbed(left_hand_side);
}

void moves_concatenation::be_absorbed(atomic_move&& left_hand_side){
    content.front().be_absorbed(std::move(left_hand_side));
}

std::vector<bracketed_move> moves_concatenation::move_out(void){
    auto result = std::move(content);
    content.clear();
    return result;
}

moves_concatenation moves_concatenation::prepare_to_split(
    std::set<token>& known_pieces,
    std::set<token>& pieces_after_split,
    uint& current_id,
    bool is_beginning,
    bool& is_end){
    std::vector<bracketed_move> result;
    for(uint i=content.size();i>0;--i){
        std::vector<bracketed_move> lower_result = content[i-1].prepare_to_split(known_pieces,pieces_after_split,current_id,is_beginning&&i==1,is_end);
        for(uint j=lower_result.size();j>0;--j)
            result.push_back(std::move(lower_result[j-1]));
    }
    std::reverse(result.begin(),result.end());
    return moves_concatenation(std::move(result));
}

bool moves_concatenation::is_epsilon(void)const{
    return content.empty();
}

void moves_concatenation::to_semisteps(
    moves_sum& N,
    moves_sum& B,
    moves_sum& T,
    moves_sum& BT,
    const std::set<token>& splitters){
    N = epsilon();
    B = T = BT = empty_expression();
    std::vector<moves_sum> Ns(content.size()),Bs(content.size()),Ts(content.size()),BTs(content.size());
    for(uint i=0;i<content.size();++i)
        content[i].to_semisteps(Ns[i],Bs[i],Ts[i],BTs[i],splitters);
    for(uint i=0;i<Ns.size();++i)
        N.concat_move(moves_sum(Ns[i]));
    if(!Bs.empty()){
        B = Bs[0];
        for(uint i=1;i<Bs.size();++i){
            B.concat_move(moves_sum(Ns[i]));
            B.add_move(moves_sum(Bs[i]));
        }
    }
    if(!Ts.empty()){
        T = Ts.back();
        for(uint i=Ts.size()-1;i>0;--i){
            moves_sum temp = std::move(T);
            T = Ns[i-1];
            T.concat_move(std::move(temp));
            T.add_move(moves_sum(Ts[i-1]));
        }
    }
    if(!BTs.empty())
        for(uint i=0;i<BTs.size();++i){
            BT.add_move(std::move(BTs[i]));
            if(i>0){
                moves_sum temp = Bs[0];
                for(uint j=1;j<i;++j){
                    temp.concat_move(moves_sum(Ns[j]));
                    temp.add_move(moves_sum(Bs[j]));
                }
                temp.concat_move(std::move(Ts[i]));
                BT.add_move(std::move(temp));
            }
        }
}

moves_sum::moves_sum(void)noexcept:
content(){}

moves_sum::moves_sum(std::vector<moves_concatenation>&& src)noexcept:
content(std::move(src)){}

parser_result<moves_sum> parse_moves_sum(
    slice_iterator& it,
    std::set<token>& encountered_pieces,
    const game_order& players,
    int player_number,
    bool& contains_turn_changer,
    messages_container& msg)throw(message){
    if(!it.has_value())
        return failure<moves_sum>();
    std::vector<moves_concatenation> result;
    bool concatenation_contains_turn_changer = false;
    parser_result<moves_concatenation> concat_result = parse_moves_concatenation(it,encountered_pieces,players,player_number,concatenation_contains_turn_changer,msg);
    contains_turn_changer |= concatenation_contains_turn_changer;
    if(concat_result.is_success())
        result.push_back(concat_result.move_value());
    else
        return failure<moves_sum>();
    while(it.has_value() && it.current().get_type() == plus){
        if(!it.next(msg))
            throw msg.build_message("Unexpected end of move sum (player \'"+players.get_player_name(player_number,0).to_string()+"\')");
        concatenation_contains_turn_changer = false;
        concat_result = parse_moves_concatenation(it,encountered_pieces,players,player_number,concatenation_contains_turn_changer,msg);
        contains_turn_changer |= concatenation_contains_turn_changer;
        if(!concat_result.is_success())
            throw msg.build_message(it.create_call_stack("Expected moves concatenation, encountered \'"+it.current().to_string()+"\'"));
        else
            result.push_back(concat_result.move_value());
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

bool moves_sum::is_goal_eligible(void)const{
    for(const auto& el: content)
        if(!el.is_goal_eligible())
            return false;
    return true;
}

void moves_sum::print_rbg(std::ostream& out,uint recurrence_depth)const{
    if(!content.empty()){
        auto it = content.begin();
        print_spaces(out,4*(recurrence_depth-1));
        it->print_rbg(out,recurrence_depth);
        out<<'\n';
        while((++it)!=content.end()){
            print_spaces(out,4*recurrence_depth-2);
            out<<"+ ";
            it->print_rbg(out,recurrence_depth);
            out<<'\n';
        }
    }
}

moves_sum moves_sum::flatten(void){
    std::vector<std::pair<std::vector<moves_concatenation>::iterator,std::vector<moves_concatenation>>> moves_stack;
    moves_stack.push_back(make_pair(content.begin(),std::move(content)));
    moves_stack.back().first = moves_stack.back().second.begin(); // inelegant way to avoid undefined behavior
    content.clear();
    std::vector<moves_concatenation> result;
    while(!moves_stack.empty()){
        auto it = moves_stack.back().first;
        if(it == moves_stack.back().second.end())
            moves_stack.pop_back();
        else{
            moves_concatenation mc = std::move(*it);
            ++moves_stack.back().first;
            mc = mc.flatten();
            if(mc.is_single_sum()){
                moves_sum next_level = mc.give_single_sum();
                moves_stack.push_back(make_pair(next_level.content.begin(),std::move(next_level.content)));
                moves_stack.back().first = moves_stack.back().second.begin(); // inelegant way to avoid undefined behavior
            }
            else
                result.push_back(std::move(mc));
        }
    }
    return moves_sum(std::move(result));
}

bool moves_sum::is_single_concatenation(void)const{
    return content.size()==1;
}

moves_concatenation moves_sum::give_single_concatenation(void){
    assert(is_single_concatenation());
    auto result = std::move(content[0]);
    content.clear();
    return result;
}

bool moves_sum::is_single_bracket(uint repetition_number_above)const{
    return content.size()==1 && content[0].is_single_bracket(repetition_number_above);
}

bracketed_move moves_sum::give_single_bracket(void){
    assert(content.size()==1);
    auto result = std::move(content[0]);
    content.clear();
    return result.give_single_bracket();
}

bool moves_sum::can_absorb(const atomic_move& right_hand_side)const{
    for(const auto& el: content)
        if(!el.can_absorb(right_hand_side))
            return false;
    return true;
}

void moves_sum::absorb(atomic_move&& right_hand_side){
    for(uint i=0;i<content.size();++i)
        content[i].absorb((i==content.size()-1 ? std::move(right_hand_side) : atomic_move(right_hand_side)));
}

bool moves_sum::can_be_absorbed(const atomic_move& left_hand_side)const{
    for(const auto& el: content)
        if(!el.can_be_absorbed(left_hand_side))
            return false;
    return true;
}

void moves_sum::be_absorbed(atomic_move&& left_hand_side){
    for(uint i=0;i<content.size();++i)
        content[i].be_absorbed((i==content.size()-1 ? std::move(left_hand_side) : atomic_move(left_hand_side)));
}

moves_sum moves_sum::prepare_to_split(
    std::set<token>& known_pieces,
    std::set<token>& pieces_after_split,
    uint& current_id,
    bool is_beginning,
    bool& is_end){
    std::vector<moves_concatenation> result;
    bool is_end_temp;
    bool is_end_final = true;
    for(uint i=0;i<content.size();++i){
        is_end_temp = is_end;
        result.push_back(content[i].prepare_to_split(known_pieces,pieces_after_split,current_id,is_beginning,is_end_temp));
        is_end_final &= is_end_temp;
    }
    is_end = is_end_final;
    return moves_sum(std::move(result));
}

bool moves_sum::is_epsilon(void)const{
    return content.size() == 1 && content.begin()->is_epsilon();
}

bool moves_sum::is_empty(void)const{
    return content.empty();
}

void moves_sum::concat_move(moves_sum&& m){
    if(is_empty()||m.is_empty())
        content.clear();
    else if(is_single_concatenation()&&m.is_single_concatenation()){
        std::vector<bracketed_move> result = give_single_concatenation().move_out();
        std::vector<bracketed_move> next_elem = m.give_single_concatenation().move_out();
        for(uint i=0;i<next_elem.size();++i)
            result.push_back(std::move(next_elem[i]));
        content.clear();
        content.push_back(std::move(result));
    }
    else{
        std::vector<bracketed_move> result;
        if(is_single_concatenation())
            result = give_single_concatenation().move_out();
        else
            result.push_back(std::move(*this));
        if(m.is_single_concatenation()){
            std::vector<bracketed_move> temp = m.give_single_concatenation().move_out();
            for(uint i=0;i<temp.size();++i)
                result.push_back(temp[i]);
        }
        else
            result.push_back(std::move(m));
        content.clear();
        content.push_back(std::move(result));
    }
}

void moves_sum::set_star(void){
    if(is_empty()||is_epsilon()){
        content.clear();
        content.push_back(std::vector<bracketed_move>());
    }
    else if(is_single_bracket(0)){
        bracketed_move lower_result = give_single_bracket();
        lower_result.set_star();
        std::vector<bracketed_move> result;
        result.push_back(std::move(lower_result));
        content.clear();
        content.push_back(std::move(result));
    }
    else{
        bracketed_move lower_result(std::move(*this));
        lower_result.set_star();
        std::vector<bracketed_move> result;
        result.push_back(std::move(lower_result));
        content.clear();
        content.push_back(std::move(result));
    }
}

void moves_sum::add_move(moves_sum&& m){
    if(is_empty())
        content = std::move(m.content);
    else
        for(uint i=0;i<m.content.size();++i)
            content.push_back(std::move(m.content[i]));
}

void moves_sum::to_semisteps(
    moves_sum& N,
    moves_sum& B,
    moves_sum& T,
    moves_sum& BT,
    const std::set<token>& splitters){
    N = B = T = BT = empty_expression();
    for(uint i=0;i<content.size();++i){
        moves_sum tempN,tempB,tempT,tempBT;
        content[i].to_semisteps(tempN,tempB,tempT,tempBT,splitters);
        N.add_move(std::move(tempN));
        B.add_move(std::move(tempB));
        T.add_move(std::move(tempT));
        BT.add_move(std::move(tempBT));
    }
}

moves_sum epsilon(void){
    std::vector<moves_concatenation> temp2;
    temp2.push_back(std::vector<bracketed_move>());
    return moves_sum(std::move(temp2));
}
moves_sum empty_expression(void){
    return moves_sum(std::vector<moves_concatenation>());
}

moves_sum single_letter(atomic_move&& m){
    std::vector<bracketed_move> temp1;
    temp1.push_back(std::move(m));
    std::vector<moves_concatenation> temp2;
    temp2.push_back(std::move(temp1));
    return moves_sum(std::move(temp2));
}

moves_sum single_letter(turn_change_indicator&& m){
    std::vector<bracketed_move> temp1;
    temp1.push_back(std::move(m));
    std::vector<moves_concatenation> temp2;
    temp2.push_back(std::move(temp1));
    return moves_sum(std::move(temp2));
}

void print_spaces(std::ostream& out,uint n){
    for(uint i=0;i<n;++i)
        out<<' ';
}

#ifndef ABSTRACT_DISPATCHER
#define ABSTRACT_DISPATCHER

namespace rbg_parser{

class sum;
class pure_sum;
class concatenation;
class pure_concatenation;
class bracketed_move;
class pure_bracketed_move;
class shift;
class ons;
class off;
class assignment;
class player_switch;
class condition_check;
class conjunction;
class alternative;
class negatable_condition;
class comparison;
class move_condition;

class abstract_dispatcher{
    public:
        virtual void dispatch(const sum&)=0;
        virtual void dispatch(const pure_sum&)=0;
        virtual void dispatch(const concatenation&)=0;
        virtual void dispatch(const pure_concatenation&)=0;
        virtual void dispatch(const bracketed_move&)=0;
        virtual void dispatch(const pure_bracketed_move&)=0;
        virtual void dispatch(const shift&)=0;
        virtual void dispatch(const ons&)=0;
        virtual void dispatch(const off&)=0;
        virtual void dispatch(const assignment&)=0;
        virtual void dispatch(const player_switch&)=0;
        virtual void dispatch(const condition_check&)=0;
        virtual void dispatch(const conjunction&)=0;
        virtual void dispatch(const alternative&)=0;
        virtual void dispatch(const negatable_condition&)=0;
        virtual void dispatch(const comparison&)=0;
        virtual void dispatch(const move_condition&)=0;
};

}

#endif

#ifndef ABSTRACT_DISPATCHER
#define ABSTRACT_DISPATCHER

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
        virtual void dispatch(sum&)=0;
        virtual void dispatch(pure_sum&)=0;
        virtual void dispatch(concatenation&)=0;
        virtual void dispatch(pure_concatenation&)=0;
        virtual void dispatch(bracketed_move&)=0;
        virtual void dispatch(pure_bracketed_move&)=0;
        virtual void dispatch(shift&)=0;
        virtual void dispatch(ons&)=0;
        virtual void dispatch(off&)=0;
        virtual void dispatch(assignment&)=0;
        virtual void dispatch(player_switch&)=0;
        virtual void dispatch(condition_check&)=0;
        virtual void dispatch(conjunction&)=0;
        virtual void dispatch(alternative&)=0;
        virtual void dispatch(negatable_condition&)=0;
        virtual void dispatch(comparison&)=0;
        virtual void dispatch(move_condition&)=0;
};

#endif

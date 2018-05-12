#ifndef ABSTRACT_DISPATCHER
#define ABSTRACT_DISPATCHER

namespace rbg_parser{

class sum;
class conditional_sum;
class concatenation;
class power_move;
class star_move;
class conditional_star_move;
class shift;
class ons;
class off;
class assignment;
class player_switch;
class keeper_switch;
class sloth;
class condition_check;
class modifier_block;
class arithmetic_comparison;
class player_check;
class integer_arithmetic;
class variable_arithmetic;
class multiply_arithmetic;
class sum_arithmetic;

class abstract_dispatcher{
    public:
        virtual void dispatch(const sum&)=0;
        virtual void dispatch(const conditional_sum&)=0;
        virtual void dispatch(const concatenation&)=0;
        virtual void dispatch(const power_move&)=0;
        virtual void dispatch(const star_move&)=0;
        virtual void dispatch(const conditional_star_move&)=0;
        virtual void dispatch(const shift&)=0;
        virtual void dispatch(const ons&)=0;
        virtual void dispatch(const off&)=0;
        virtual void dispatch(const assignment&)=0;
        virtual void dispatch(const player_switch&)=0;
        virtual void dispatch(const keeper_switch&)=0;
        virtual void dispatch(const sloth&)=0;
        virtual void dispatch(const condition_check&)=0;
        virtual void dispatch(const modifier_block&)=0;
        virtual void dispatch(const arithmetic_comparison&)=0;
        virtual void dispatch(const player_check&)=0;
        virtual void dispatch(const integer_arithmetic&)=0;
        virtual void dispatch(const variable_arithmetic&)=0;
        virtual void dispatch(const multiply_arithmetic&)=0;
        virtual void dispatch(const sum_arithmetic&)=0;
        virtual ~abstract_dispatcher(void)=default;
};

}

#endif

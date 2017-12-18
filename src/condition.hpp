#ifndef CONDITION
#define CONDITION

class abstract_dispatcher;
// interface
class condition{
    public:
        virtual ~condition(void)=default;
        virtual void accept(abstract_dispatcher &dispatcher) = 0;
};

#endif

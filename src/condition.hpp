#ifndef CONDITION
#define CONDITION

#include<memory>

class abstract_dispatcher;
// interface
class condition{
    public:
        virtual ~condition(void)=default;
        virtual void accept(abstract_dispatcher &dispatcher)=0;
        virtual std::unique_ptr<condition> simplify(void)=0;
};

#endif

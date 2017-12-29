#ifndef CONDITION
#define CONDITION

#include<memory>

class abstract_dispatcher;
// interface
class condition{
    public:
        virtual ~condition(void)=default;
        virtual void accept(abstract_dispatcher &dispatcher)const=0;
        virtual std::unique_ptr<condition> simplify(void)=0;
        virtual uint priority(void)const=0; // being of higher priority containg lower ones requires surrounding them with brackets
        virtual std::string to_rbg(uint indent)const=0;
        virtual std::string to_rbg()const=0;
};

#endif

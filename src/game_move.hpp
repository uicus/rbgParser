#ifndef GAME_MOVE
#define GAME_MOVE

#include<memory>
#include<vector>
#include<set>

#include"straightness_helpers.hpp"
#include"token.hpp"
#include"condition.hpp"

namespace rbg_parser{

class abstract_dispatcher;
// interface
class game_move : public condition{
    public:
        virtual ~game_move(void)=default;
        virtual bool modifies(void)const{return true;}
        virtual std::unique_ptr<game_move> flatten(void)=0;
        std::unique_ptr<condition> condition_flatten(void)override{return flatten();};
        virtual std::unique_ptr<game_move> simplify(void)=0;
        std::unique_ptr<condition> condition_simplify(void)override{return simplify();};
        virtual void gather_concatenation_elements(
            std::vector<std::unique_ptr<game_move>>& elements,
            std::vector<std::unique_ptr<game_move>>& next_block_elements);
        virtual void gather_sum_elements(std::vector<std::unique_ptr<game_move>>& elements){elements.push_back(flatten());};
        virtual straightness_result compute_k_straightness(void)const{return standard_non_switch();};
        virtual bool check_if_redundant(std::set<token>&, bool&)const{return false;};
        virtual bool has_finisher(void)const{return false;};
};

}

#endif

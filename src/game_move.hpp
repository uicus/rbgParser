#ifndef GAME_MOVE
#define GAME_MOVE

#include<memory>
#include<vector>
#include<set>

#include"straightness_helpers.hpp"
#include"token.hpp"

namespace rbg_parser{

class abstract_dispatcher;
class options;
// interface
class game_move{
    protected:
        int index = 0;
    public:
        virtual ~game_move(void)=default;
        int index_in_expression(void)const{return index;};
        virtual int give_indices_in_expression(int next_free)=0;
        virtual bool modifies(void)const{return true;}
        virtual bool is_modifier(void)const{return false;}
        virtual std::unique_ptr<game_move> flatten(void)=0;
        virtual std::unique_ptr<game_move> simplify(void)=0;
        virtual void accept(abstract_dispatcher& dispatcher)const=0;
        virtual uint priority(const options&)const=0; // being of higher priority containing lower ones requires surrounding them with brackets
        virtual std::string to_rbg(const options& opts,uint indent)const=0;
        virtual std::string to_rbg(const options&)const=0;
        virtual void gather_concatenation_elements(std::vector<std::unique_ptr<game_move>>& elements){elements.push_back(flatten());}
        virtual void gather_sum_elements(std::vector<std::unique_ptr<game_move>>& elements){elements.push_back(flatten());};
        virtual straightness_result compute_k_straightness([[maybe_unused]] StraightnessType st)const{return standard_non_switch();};
        virtual bool check_if_redundant(std::set<token>&, bool&)const{return false;};
        virtual bool has_finisher(void)const{return false;};
        virtual bool has_piece_as_variable(const std::set<token>&)const{return false;};
        virtual std::unique_ptr<game_move> copy(void)const=0;
};

}

#endif

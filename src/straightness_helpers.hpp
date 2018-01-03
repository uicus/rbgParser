#ifndef STRAIGHTNESS_HELPERS
#define STRAIGHTNESS_HELPERS

namespace rbg_parser{


inline int repeat(int previous_result, int times){
    if(times==0 && previous_result>0)
        return -2;
    return previous_result*times;
}

inline int count_max_of_straightness(int previous_result,int current_result){
    if(previous_result==-2 || current_result==-2)
        return -2;
    if(previous_result==-1)
        return current_result;
    if(current_result==-1)
        return current_result;
    return std::max(previous_result,current_result);
}

inline int append_results(int r1, int r2){
    if(r1==-2 || r2==-2)
        return -2;
    if(r1==-1 || r2==-1)
        return -1;
    return r1+r2;
}

// maximum of modifiers in move
// -1 = couldn't construct any move given type
// -2 = infinity
// non-negative = count
struct straightness_result{
    int self_contained_moves;
    int beginning_moves;
    int ending_moves;
    int only_coming_through_moves;
    void repeat_result(int times){
        repeat(self_contained_moves,times);
        repeat(beginning_moves,times);
        repeat(ending_moves,times);
        repeat(only_coming_through_moves,times);
    }
    void max_of_results(const straightness_result& rhs){
        self_contained_moves = count_max_of_straightness(self_contained_moves,rhs.self_contained_moves);
        beginning_moves = count_max_of_straightness(beginning_moves,rhs.beginning_moves);
        ending_moves = count_max_of_straightness(ending_moves,rhs.ending_moves);
        only_coming_through_moves = count_max_of_straightness(only_coming_through_moves,rhs.only_coming_through_moves);
    }
};

inline straightness_result standard_non_switch(void){
    return {-1,-1,-1,0};
}

inline straightness_result modifier_non_switch(void){
    return {-1,-1,-1,1};
}

inline straightness_result standard_switch(void){
    return {0,0,0,-1};
}

inline straightness_result empty_move(void){
    return {-1,-1,-1,-1};
}

}

#endif

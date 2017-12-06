#ifndef PURE_SUM
#define PURE_SUM

#include<vector>
#include<memory>

#include"pure_game_move.hpp"

class sum;

class pure_sum : public pure_game_move{
        std::vector<std::unique_ptr<pure_game_move>> content;
        pure_sum(void)=default;
        pure_sum(std::vector<std::unique_ptr<pure_game_move>>&& content);
    public:
        ~pure_sum(void)override=default;
        pure_sum(pure_sum&&)=default;
        pure_sum(const pure_sum&)=default;
        pure_sum& operator=(const pure_sum&)=default;
        pure_sum& operator=(pure_sum&&)=default;
        std::unique_ptr<pure_game_move> transform_into_pure(void)override;
        friend class sum;
};
#endif

#ifndef PURE_CONCATENATION
#define PURE_CONCATENATION

#include<vector>
#include<memory>

#include"pure_game_move.hpp"

class concatenation;

class pure_concatenation : public pure_game_move{
        std::vector<std::unique_ptr<pure_game_move>> content;
        pure_concatenation(void)=default;
        pure_concatenation(std::vector<std::unique_ptr<pure_game_move>>&& content);
    public:
        ~pure_concatenation(void)override=default;
        pure_concatenation(pure_concatenation&&)=default;
        pure_concatenation(const pure_concatenation&)=default;
        pure_concatenation& operator=(const pure_concatenation&)=default;
        pure_concatenation& operator=(pure_concatenation&&)=default;
        std::unique_ptr<pure_game_move> transform_into_pure(void)override;
        friend class concatenation;
};

#endif

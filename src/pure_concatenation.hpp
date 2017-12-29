#ifndef PURE_CONCATENATION
#define PURE_CONCATENATION

#include<vector>
#include<memory>

#include"pure_game_move.hpp"
#include"abstract_dispatcher.hpp"

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
        std::unique_ptr<pure_game_move> pure_simplify(void)override;
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(void)const override{return 1;};
        std::string to_rbg(uint indent)const override;
        std::string to_rbg()const override;
        const std::vector<std::unique_ptr<pure_game_move>>& get_content(void)const;
        friend class concatenation;
};

#endif

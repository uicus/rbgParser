#ifndef ONS
#define ONS

#include<set>
#include<vector>

#include"game_move.hpp"
#include"abstract_dispatcher.hpp"

namespace rbg_parser{

class ons : public game_move{
        std::set<token> legal_ons;
        ons(void)=default;
    public:
        ons(std::vector<token>&& legal_ons);
        ~ons(void)override=default;
        ons(ons&&)=default;
        ons(const ons&)=delete;
        ons& operator=(const ons&)=delete;
        ons& operator=(ons&&)=default;
        int give_indices_in_expression(int next_free)override;
        std::unique_ptr<game_move> simplify(void)override{return std::unique_ptr<game_move>(new ons(std::move(*this)));};
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(const options&)const override{return 4;};
        std::string to_rbg(const options& opt, uint)const override;
        std::string to_rbg(const options&)const override;
        std::unique_ptr<game_move> flatten(void)override;
        const std::set<token>& get_legal_ons(void)const;
        std::unique_ptr<game_move> copy(void)const override;
};

}

#endif

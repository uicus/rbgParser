#ifndef SHIFT
#define SHIFT

#include<memory>

#include"game_move.hpp"
#include"abstract_dispatcher.hpp"
#include"token.hpp"

namespace rbg_parser{

class shift : public game_move{
        token edge_name;
        shift(void)=default;
    public:
        shift(const token& edge_name);
        ~shift(void)override=default;
        shift(shift&&)=default;
        shift(const shift&)=delete;
        shift& operator=(const shift&)=delete;
        shift& operator=(shift&&)=default;
        int give_indices_in_expression(int next_free)override;
        std::unique_ptr<game_move> simplify(void)override{return std::unique_ptr<game_move>(new shift(std::move(*this)));};
        void accept(abstract_dispatcher& dispatcher)const override;
        uint priority(const options&)const override{return 4;};
        std::string to_rbg(const options& opt, uint)const override;
        std::string to_rbg(const options&)const override;
        std::unique_ptr<game_move> flatten(void)override;
        const token& get_content(void)const;
        std::unique_ptr<game_move> copy(void)const override;
};

}

#endif

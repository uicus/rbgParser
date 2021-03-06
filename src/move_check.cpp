#include"move_check.hpp"
#include"options.hpp"

namespace rbg_parser{

move_check::move_check(std::unique_ptr<game_move>&& content, bool negated):
content(std::move(content)),
negated(negated){
}

int move_check::give_indices_in_expression(int next_free){
    index = next_free;
    return next_free;
}

void move_check::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

const game_move* move_check::get_content(void)const{
    return content.get();
}

bool move_check::is_negated(void)const{
    return negated;
}

std::unique_ptr<game_move> move_check::simplify(void){
    return std::unique_ptr<game_move>(new move_check(content->simplify(), negated));
}

straightness_result move_check::compute_k_straightness(StraightnessType st)const{
    if (st != StraightnessType::APP_STRAIGHTNESS) return standard_non_switch();
    auto result = content->compute_k_straightness(st);
    result.wrap_in_check();
    return result;
}

std::string move_check::to_rbg(const options& opt, uint indent)const{
    std::string result = "";
    result += negated ? "{!" : "{?";
    result += content->to_rbg(opt.create_inside_pattern_copy(), indent);
    result += "}";
    return result;
}

std::string move_check::to_rbg(const options& opt)const{
    std::string result = "";
    result += negated ? "{!" : "{?";
    result += content->to_rbg(opt.create_inside_pattern_copy());
    result += "}";
    return result;
}

std::unique_ptr<game_move> move_check::flatten(void){
    return std::unique_ptr<game_move>(new move_check(content->flatten(), negated));
}

std::unique_ptr<game_move> move_check::copy(void)const{
    return std::unique_ptr<game_move>(new move_check(content->copy(),negated));
}

bool move_check::has_piece_as_variable(const std::set<token>& pieces)const{
    return content->has_piece_as_variable(pieces);
}

}

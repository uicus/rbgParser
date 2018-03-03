#include"condition_check.hpp"

namespace rbg_parser{

condition_check::condition_check(std::unique_ptr<condition> content, bool negated):
content(std::move(content)),
negated(negated){
}

void condition_check::accept(abstract_dispatcher& dispatcher)const{
    dispatcher.dispatch(*this);
}

const condition* condition_check::get_content(void)const{
    return content.get();
}

bool condition_check::is_negated(void)const{
    return negated;
}

std::unique_ptr<game_move> condition_check::simplify(void){
    return std::unique_ptr<game_move>(new condition_check(content->condition_simplify(), negated));
}

std::string condition_check::to_rbg(uint indent)const{
    std::string result = "";
    result += negated ? "(?" : "(!";
    result += content->to_rbg(indent);
    result += ")";
    return result;
}

std::string condition_check::to_rbg()const{
    std::string result = "";
    result += negated ? "(?" : "(!";
    result += content->to_rbg();
    result += ")";
    return result;
}

std::unique_ptr<game_move> condition_check::flatten(void){
    return std::unique_ptr<game_move>(new condition_check(content->condition_flatten(), negated));
}

}

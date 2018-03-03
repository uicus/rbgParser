#include"multiply_arithmetic.hpp"
#include"abstract_dispatcher.hpp"
#include"printer_helpers.hpp"

namespace rbg_parser{

multiply_arithmetic::multiply_arithmetic(std::vector<std::unique_ptr<arithmetic_expression>>&& content):
content(std::move(content)){
}

void multiply_arithmetic::accept(abstract_dispatcher &dispatcher)const{
    dispatcher.dispatch(*this);
}

std::unique_ptr<arithmetic_expression> multiply_arithmetic::simplify(void){
    std::vector<std::unique_ptr<arithmetic_expression>> result;
    for(uint i=0;i<content.size();++i)
        result.push_back(content[i]->simplify());
    if(result.size()==1)
        return std::move(result[0]);
    else
        return std::unique_ptr<arithmetic_expression>(new multiply_arithmetic(std::move(result)));
}

std::unique_ptr<arithmetic_expression> multiply_arithmetic::flatten(void){
    std::vector<std::unique_ptr<arithmetic_expression>> result;
    for(auto& el: content)
        el->gather_multiply_elements(result);
    return std::unique_ptr<arithmetic_expression>(new multiply_arithmetic(std::move(result)));
}

void multiply_arithmetic::gather_multiply_elements(std::vector<std::unique_ptr<arithmetic_expression>>& elements){
    for(auto& el: content)
        el->gather_multiply_elements(elements);
}

std::string multiply_arithmetic::to_rbg(uint)const{
    return to_rbg();
}

std::string multiply_arithmetic::to_rbg()const{
    std::string result = "";
    for(uint i=0;i<content.size();++i){
        result += (i==0 ? "" : "*");
        result += open_bracket_if_necessary(priority(),content[i]->priority());
        result += content[i]->to_rbg();
        result += close_bracket_if_necessary(priority(),content[i]->priority());
    }
    return result;
}

const std::vector<std::unique_ptr<arithmetic_expression>>& multiply_arithmetic::get_content(void)const{
    return content;
}

}

#ifndef PARSER_HELPERS
#define PARSER_HELPERS

#include<vector>

#include"slice_iterator.hpp"
#include"token.hpp"
#include"message.hpp"

namespace rbg_parser{

template<typename T>
class parser_result;

template<typename T>
parser_result<T> failure(void);

template<typename T>
parser_result<T> success(T&& result);

template<typename T>
class parser_result{
        bool ok;
        T value;
        parser_result(void):ok(false),value(){}
        parser_result(T&& result):ok(true),value(std::move(result)){}
    public:
        friend parser_result failure<T>(void)noexcept;
        friend parser_result success<T>(T&& result)noexcept;
        bool is_success(void)const{return ok;}
        const T& get_value(void)const{return value;}
        T move_value(void){auto a = std::move(value);value = T();return a;}
};

template<typename T>
parser_result<T> failure(void){
    return parser_result<T>();
}

template<typename T>
parser_result<T> success(T&& result){
    return parser_result<T>(std::move(result));
}

parser_result<std::vector<token>> parse_sequence(
    slice_iterator& it,
    const std::string& purpose_name,
    const std::set<token>& verification_set,
    bool should_verify,
    messages_container& msg
    )throw(message);

}

#endif

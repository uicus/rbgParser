#ifndef PARSER_HELPERS
#define PARSER_HELPERS

#include<set>

#include"slice_iterator.hpp"
#include"token.hpp"
#include"message.hpp"

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
        friend parser_result failure<T>(void);
        friend parser_result success<T>(T&& result);
        bool is_success(void)const{return ok;}
        const T& get_value(void)const{return value;}
};

template<typename T>
parser_result<T> failure(void){
    return parser_result<T>();
}

template<typename T>
parser_result<T> success(T&& result){
    return parser_result<T>(std::move(result));
}

parser_result<int> parse_int(slice_iterator& it, messages_container& msg)throw(message);
parser_result<std::set<token>> parse_tokens_set(slice_iterator& it, messages_container& msg)throw(message);

#endif

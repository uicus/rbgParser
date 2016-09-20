#ifndef SLICE_ITERATOR
#define SLICE_ITERATOR

#include<vector>
#include<string>

#include"slice.hpp"
#include"token.hpp"
#include"macro_bank.hpp"
#include"message.hpp"
#include"types.hpp"

class clipboard{
        token stored;
        bool up_to_date : 1;
        bool frozen : 1;
    public:
        clipboard(void);

        void paste(const token& t)throw(std::string);
        void report_end(void);
        void report_next_token(void);
        void paste_null_token(void);
        bool contains_full_token(void)const;
        bool contains_some_token(void)const;
        bool should_paste_into(void)const;
        token get_current_token()const;
};

class backtrace_info{
        const slice current_slice;
        uint current_begin;
        uint current_end;
        bool started : 1;
        bool last_should_be_pasted : 1;
    public:
        backtrace_info(const slice& s);
        backtrace_info(const backtrace_info& src);
        backtrace_info& operator=(const backtrace_info&)=delete;
        backtrace_info(backtrace_info&& src);
        backtrace_info& operator=(backtrace_info&&)=delete;
        ~backtrace_info(void);

        std::vector<slice> parse_arguments(messages_container& msg)throw(message);
        void rewind_parsing_arguments(void);
        void set_last_should_be_pasted(void);
        bool get_last_should_be_pasted(void);
        uint get_context_order(void)const;
        bool points_at_variable(void)const;
        bool next_is_tilde(void)const;
        bool is_last_token_in_slice(void)const;
        bool is_first_token_in_slice(void)const;
        slice get_variable_slice(void)const;
        token get_beginning(void)const;
        token current(void)const;
        void next(void);
        bool has_value(void)const;
};

class slice_iterator{
        std::vector<backtrace_info> backtrace_stack;
        clipboard c;
    public:
        slice_iterator(const slice& s);

        std::vector<std::pair<uint,std::string>> create_call_stack(const std::string& details)const;
        bool has_value(void)const;
        token current(void)const;
        void push_next_slice(slice&& s);
        void pop_slice(void);
        bool handle_standard_token(messages_container& msg)throw(message);
        bool next(const macro_bank& macros,messages_container& msg)throw(message); // true -> moved forward, false -> end of iterator
};

#endif

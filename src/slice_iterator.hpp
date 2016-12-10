#ifndef SLICE_ITERATOR
#define SLICE_ITERATOR

#include<vector>
#include<string>
#include<memory>

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
        const token& get_current_token()const;
};

class backtrace_info{
        std::shared_ptr<backtrace_info> parent_slice;
        const slice current_slice;
        uint parent_begin;
        uint parent_end;
        bool last_should_be_pasted : 1;
    public:
        backtrace_info(const slice& s,const std::shared_ptr<backtrace_info>& parent,uint current_begin,uint current_end);
        backtrace_info(const backtrace_info& src);
        backtrace_info& operator=(const backtrace_info&)=delete;
        backtrace_info(backtrace_info&& src);
        backtrace_info& operator=(backtrace_info&&)=delete;
        ~backtrace_info(void);

        std::vector<slice> parse_arguments(messages_container& msg,uint current_begin,uint& current_end)throw(message);
        std::shared_ptr<backtrace_info> get_parent(void)const;
        backtrace_info* get_pointer(void)const;
        uint get_begin(void)const;
        uint get_end(void)const;
        void set_last_should_be_pasted(void);
        bool get_last_should_be_pasted(void);
        uint get_context_order(void)const;
        bool points_at_variable(uint current_end)const;
        bool next_is_tilde(uint current_end)const;
        bool is_last_token_in_slice(uint current_end)const;
        const slice& get_variable_slice(uint current_end)const;
        const token& current(uint current_end)const;
        bool has_value(uint current_end)const;
};

class slice_iterator{
        std::shared_ptr<backtrace_info> top;
        clipboard c;
        bool started;
        uint current_begin;
        uint current_end;
        uint base_context_order;
        const macro_bank* macros;
        void push_next_slice(const slice& s);
        void pop_slice(void);
        bool handle_standard_token(messages_container& msg)throw(message);
        void move_cursor(void);
    public:
        slice_iterator(const slice& s, const macro_bank* macros);
        slice_iterator& operator=(const slice_iterator&)=delete;
        slice_iterator& operator=(slice_iterator&&)=delete;
        slice_iterator(const slice_iterator& src);
        slice_iterator(slice_iterator&& src);

        std::vector<std::pair<uint,std::string>> create_call_stack(const std::string& details)const;
        bool has_value(void)const;
        const token& current(void)const;
        bool next(messages_container& msg)throw(message); // true -> moved forward, false -> end of iterator
};

#endif

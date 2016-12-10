#ifndef SLICE
#define SLICE

#include<map>
#include<vector>

#include"types.hpp"
#include"token.hpp"

class slice{
        const std::vector<token>* data; // doesn't own the data
        uint begin;
        uint end;
        bool replacements_owner;
        std::map<token,slice>* replacements;
        uint context_order;
        slice(const slice& src, uint from, uint to);
    public:
        slice(const std::vector<token>* data, uint begin, uint end, uint context_order);
        slice(const slice& src);
        slice(slice&& src);
        slice& operator=(const slice&)=delete;
        slice& operator=(slice&&)=delete;
        ~slice(void);

        slice cut_slice(uint from, uint to)const; // relative coords
        void add_replacement(const token& ident,const slice& value);
        bool should_be_replaced(const token& ident)const;
        const slice& get_replacement(const token& ident)const;
        bool is_empty(void)const;

        uint get_context_order(void)const;
        const token& get_token(uint index)const;
        bool is_end(uint index)const;
};

#endif

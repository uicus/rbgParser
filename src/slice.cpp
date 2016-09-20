#include<cassert>

#include"slice.hpp"

slice::slice(const slice& src, uint from, uint to):
data(src.data),
begin(src.begin+from),
end(src.begin+to),
replacements(src.replacements),
context_order(src.context_order){
}

slice::slice(const std::vector<token>* d, uint b, uint e, uint co):
data(d),
begin(b),
end(e),
replacements(),
context_order(co){
    assert(begin<=end && end <= d->size());
}

slice::slice(const slice& src):
data(src.data),
begin(src.begin),
end(src.end),
replacements(src.replacements),
context_order(src.context_order){
}

slice::slice(slice&& src):
data(src.data),
begin(src.begin),
end(src.end),
replacements(std::move(src.replacements)),
context_order(src.context_order){
}

slice::~slice(void){
}

slice slice::cut_slice(uint from, uint to)const{
    assert(from<=to && begin+to<=end);
    return slice(*this, from, to);
}

void slice::add_replacement(const token& ident,const slice& value){
    replacements.insert(std::make_pair(ident,std::move(value)));
}

bool slice::should_be_replaced(const token& ident)const{
    return replacements.find(ident) != replacements.end();
}

slice slice::get_replacement(const token& ident)const{
    return replacements.find(ident)->second;
}

bool slice::is_empty(void)const{
    return end <= begin;
}

uint slice::get_context_order(void)const{
    return context_order;
}

token slice::get_token(uint index)const{
    assert(begin+index<end);
    return (*data)[begin+index];
}

bool slice::is_end(uint index)const{
    return begin+index >= end;
}

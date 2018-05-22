#ifndef RULES_PARSER
#define RULES_PARSER

#include<memory>
#include"message.hpp"

namespace rbg_parser{

class expression;
class slice_iterator;

std::unique_ptr<expression> parse_rules(slice_iterator& it, messages_container& msg);

}

#endif

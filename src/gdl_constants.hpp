#ifndef GDL_CONSTANTS
#define GDL_CONSTANTS

#include<string>

const std::string separator = ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n";
const std::string no_off = "no_off";
const std::string semi_turn = "semi_turn";

inline std::string section_title(std::string&& name);
inline uint length_of(uint v);
inline std::string succ(const std::string& name, uint max_number, bool logarithmic=false);
inline std::string any_number(const std::string& digits_name,uint max_number,bool logarithmic=false);
inline std::string unary_binary_equivalency(const std::string& name, int max_binary_number, int max_equivalent_number);

inline std::string section_title(std::string&& name){
    return separator+";; "+std::move(name)+'\n'+separator;
}

inline uint length_of(uint v){
    uint i;
    for(i=0;v>>=1;++i);
    return i+1;
}

const std::string digit = "(digit 0)\n(digit 1)\n";

inline std::string succ(const std::string& name, uint max_number, bool logarithmic){
    std::string result;
    if(logarithmic){
        uint length = length_of(max_number);
        for(uint i=0;i<length;++i){
            if(i<length-1)
                result+="(<= ";
            result+="("+name;
            for(uint j=length-1;j>i;--j)
                result+=" ?x"+std::to_string(j);
            result+=" 0";
            for(uint j=i;j>0;--j)
                result+=" 1";
            for(uint j=length-1;j>i;--j)
                result+=" ?x"+std::to_string(j);
            result+=" 1";
            for(uint j=i;j>0;--j)
                result+=" 0";
            result+=")";
            for(uint j=length-1;j>i;--j)
                result+="\n\t(digit ?x"+std::to_string(j)+')';
            if(i<length-1)
                result+=')';
            result+='\n';
        }
    }
    else
        for(uint i=1;i<=max_number;++i)
            result += "("+name+' '+std::to_string(i)+' '+std::to_string(i+1)+")\n";
    return result;
}

inline std::string any_number(const std::string& digits_name,uint max_number,bool logarithmic){
    std::string result = "";
    if(logarithmic){
        uint l = length_of(max_number);
        for(uint i=0;i<l;++i)
            result+=" ?"+digits_name+std::to_string(i);
    }
    else
        result+=" ?"+digits_name;
    return result;
}

inline std::string unary_binary_equivalency(const std::string& name, int max_binary_number, int max_equivalent_number){
    std::string result;
    uint l = length_of(max_binary_number);
    uint max_significant_length = length_of(max_equivalent_number);
    uint current_number = 0;
    while(!(current_number&(1<<(max_significant_length)))){
        result+="("+name+" "+std::to_string(current_number);
        for(uint i=l;i>0;--i)
            result+=(current_number&(1<<(i-1))?" 1":" 0");
        result+=")\n";
        ++current_number;
    }
    for(++max_significant_length;max_significant_length<=l;++max_significant_length){
        result+="("+name+" "+std::to_string(current_number);
        for(uint i=0;i<l-max_significant_length;++i)
            result+=" 0";
        result+=" 1";
        for(uint i=0;i+1<max_significant_length;++i)
            result+=" ?s"+std::to_string(i);
        result+=")\n";
    }
    return result;
}

#endif

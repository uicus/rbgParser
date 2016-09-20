#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

#include"options.hpp"
#include"token.hpp"
#include"message.hpp"
#include"game_items.hpp"
#include"macro_bank.hpp"
#include"slice_iterator.hpp"

int main(int argc, const char** argv){
    if(argc < 2){
        std::cerr<<"No input specified"<<std::endl;
        std::cerr<<"Usage: \"rbg2gdl [flags] input_file\""<<std::endl;
        std::cerr<<"Available flags:"<<std::endl;
        std::cerr<<"\"-o output_file\" - write output to file with given name; defaults to \"a.gdl\""<<std::endl;
        std::cerr<<"\"-v\" - just verify input; do not generate output file"<<std::endl;
        std::cerr<<"\"-Whide\" - do not show warnings"<<std::endl;
        std::cerr<<"\"-Werror\" - treat warnings as errors"<<std::endl;
    }
    else{
        std::string input_file_name(argv[argc-1]);
        options o(argc-2, argv+1);
        std::ifstream t(input_file_name);
        std::stringstream buffer;
        buffer << t.rdbuf();
        messages_container msg;
        try{
            std::vector<token> result = tokenize(buffer.str(),msg);
            game_items g = input_tokens(result,msg);
            slice_iterator it = g.get_game_segment();
            const macro_bank& macros = g.get_macros();
            it.next(macros,msg);
            while(it.has_value()){
                std::cout<<it.current().to_string()<<' ';
                it.next(macros,msg);
            }
            std::cout<<std::endl;
        }
        catch(message& m){
            std::cout<<m.as_error()<<std::endl;
        }
        msg.write_as_warnings(std::cout);
    }
    return 0;
}

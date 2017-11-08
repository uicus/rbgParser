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
        std::cerr<<"\"-p\" - just preprocess"<<std::endl;
        std::cerr<<"\"-v\" - just verify input; do not generate output file"<<std::endl;
        std::cerr<<"\"-s\" - translate to simple form"<<std::endl;
        std::cerr<<"\"-g\" - translate to gdl (default)"<<std::endl;
        std::cerr<<"\"-Whide\" - do not show warnings"<<std::endl;
        std::cerr<<"\"-Werror\" - treat warnings as errors"<<std::endl;
        std::cerr<<"\"-unfold\" - unfold moves in simple form if they are under ^n"<<std::endl;
        std::cerr<<"\"-skip-comments\" - do not generate comments in *.gdl"<<std::endl;
        std::cerr<<"\"-skip-base\" - do not generate base predicates in *.gdl"<<std::endl;
        std::cerr<<"\"-skip-input\" - do not generate input predicates in *.gdl"<<std::endl;
    }
    else{
        messages_container msg;
        try{
            std::string input_file_name(argv[argc-1]);
            options o(argc-2, argv+1);
            std::ifstream t(input_file_name);
            std::stringstream buffer;
            buffer << t.rdbuf();
            std::ofstream out(o.output_file());
            if(!out.good())
                throw msg.build_message("Couldn't open file "+o.output_file());
            std::vector<token> result = tokenize(buffer.str(),msg);
            game_items g = input_tokens(result,msg);
            if(o.just_preprocess())
                g.print_rbg(out,msg);
            /*else{
                parsed_game pg = g.parse_game(msg);
                pg.to_simple(o);
                if(o.just_semisteps())
                    out<<pg;
                else
                    pg.to_gdl(out,o);
            }*/
        }
        catch(message& m){
            std::cout<<m.as_error()<<std::endl;
        }
        catch(std::exception& e){
            std::cout<<e.what()<<std::endl;
        }
        msg.write_as_warnings(std::cout);
    }
    return 0;
}

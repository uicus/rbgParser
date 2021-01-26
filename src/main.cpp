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
        std::cerr<<"\"-o output_file\" - write output to file with given name; defaults to \"a.rbg\""<<std::endl;
        std::cerr<<"\"-Whide\" - do not show warnings"<<std::endl;
        std::cerr<<"\"-Werror\" - treat warnings as errors"<<std::endl;
        std::cerr<<"\"-fnoop-before-alternative\" - put dots before alternative expressions in output mode (e.g. turn '(a + b)' into '. (a + b)')"<<std::endl;
        std::cerr<<"\"-fnoop-after-alternative\" - put dots inside alternative expressions in output mode (e.g. turn '(a + b)' into '(. a + . b)')"<<std::endl;
        std::cerr<<"\"-fnoop-before-modifier\" - put dots before modifiers in output mode (e.g. turn '[a]' into '. [a]')"<<std::endl;
        std::cerr<<"\"-fnoop-after-modifier\" - put dots after modifiers in output mode (e.g. turn '[a]' into '[a] .')"<<std::endl;
        std::cerr<<"\"-fnoop-before-star\" - put dots before stars expressions in output mode (e.g. turn 'a*' into '. a*')"<<std::endl;
        std::cerr<<"\"-fnoop-after-star\" - put dots inside stars expressions in output mode (e.g. turn 'a*' into '(. a)*')"<<std::endl;
    }
    else{
        rbg_parser::messages_container msg;
        try{
            std::string input_file_name(argv[argc-1]);
            rbg_parser::options o(argc-2, argv+1);
            std::ifstream t(input_file_name);
            std::stringstream buffer;
            buffer << t.rdbuf();
            std::ofstream out(o.output_file());
            if(!out.good())
                throw msg.build_message("Couldn't open file "+o.output_file());
            std::vector<rbg_parser::token> result = rbg_parser::tokenize(buffer.str(),msg);
            rbg_parser::game_items g = rbg_parser::input_tokens(result,msg);
            rbg_parser::parsed_game pg = g.parse_game(msg);
            if(not o.escalating_warnings() or msg.is_empty()){
                if(msg.is_empty() or not o.showing_warnings())
                    std::cout<<"Verification successful"<<std::endl;
                else
                    msg.write_as_warnings(std::cout);
                std::cout<<"Applicational strong straightness: "<<pg.get_moves()->compute_k_straightness(rbg_parser::StraightnessType::APP_STRAIGHTNESS).final_result(rbg_parser::StraightnessType::APP_STRAIGHTNESS)<<std::endl;
                std::cout<<"Main straightness: "<<pg.get_moves()->compute_k_straightness(rbg_parser::StraightnessType::MAIN_STRAIGHTNESS).final_result(rbg_parser::StraightnessType::MAIN_STRAIGHTNESS)<<std::endl;
                std::cout<<"Move straightness: "<<pg.get_moves()->compute_k_straightness(rbg_parser::StraightnessType::MOVE_STRAIGHTNESS).final_result(rbg_parser::StraightnessType::MOVE_STRAIGHTNESS)<<std::endl;
                std::cout<<"Semimove straightness: "<<pg.get_moves()->compute_k_straightness(rbg_parser::StraightnessType::SEMIMOVE_STRAIGHTNESS).final_result(rbg_parser::StraightnessType::SEMIMOVE_STRAIGHTNESS)<<std::endl;
                out<<pg.to_rbg(o, true);
            }
            else
                msg.write_as_errors(std::cout);
        }
        catch(rbg_parser::message& m){
            std::cout<<m.as_error()<<std::endl;
        }
        catch(std::exception& e){
            std::cout<<e.what()<<std::endl;
        }
    }
    return 0;
}

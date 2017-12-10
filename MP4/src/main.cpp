/*
 *  main.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/14/17
 *  Purpose:
 *
 */


// standard libraries
#include <stdio.h>
#include <chrono>
#include <random>

// other standard imported libraries
#include <allegro5/allegro.h>

// custom libraries
#include "utility/MessageException.hpp"
#include "utility/custom_exception.hpp"
#include "utility/text_color.hpp"
#include "utility/commandline_parser.hpp"
#include "utility/config_parser.hpp"

// pong related code
#include "game/pong_bundle.hpp"
#include "RL/pong_training.hpp"


int main(int argc, char** argv){
    
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    try {
        
        // get command line inputs
        parser::commandline commp(argc,argv);
        parser::config pconfig;
        
        if( commp["-config"].size() != 0 ){
            pconfig.parse(commp["-config"]);
        }else{
            throw custom::exception("Need to pass in config file via command line input `-config` to setup game/training.");
        }
        
        if( pconfig.retrieve<std::string>("goal") == "play" ){
            pong::initAllegro();
            pong::bundle game_play(700,600,20);
            game_play.setConfig(pconfig);
            game_play.run();
        }else{
            pong::train(pconfig);
        }
    
    }catch( MessageException & msg ){
        text::printf_color(text::Cyan, "Exception: ");
        msg.msg();
        text::printf_color(text::Red, "\n");
        return -1;
        
    }catch( std::exception & msg ){
        text::printf_color(text::Cyan, "Exception: ");
        text::printf_color(text::Red, msg.what());
        text::printf_color(text::Red, "\n");
        return -2;
    }
    
    // write out run time for everything
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    auto time_span = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    printf("The code ran for %lf seconds\n", (double)(time_span.count()/1000.0) );
    
	return 0;
}

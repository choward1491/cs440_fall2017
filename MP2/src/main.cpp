/*
 *  main.hpp
 *
 *  Project: MP2
 *  Author : Christian J Howard
 *  Date   : 10/10/17
 *  Purpose:
 *
 */




// standard libs
#include <stdio.h>
#include <chrono>

// exception related stuff
#include "commandline_parser.hpp"
#include "MessageException.hpp"
#include "custom_exception.hpp"
#include "text_color.hpp"
#include "test_csp.h"

// game playing agent stuff
#include "breakthrough_rules.hpp"
#include "breakthrough_provided_heuristics.hpp"
#include "game_instance.hpp"
#include "breakthrough_minimax_agent.hpp"

#define NR 8
#define NC 8

typedef bt::baseline_rules<NR,NC> bt_rules;
typedef game::agent<bt_rules>     bt_agent;
typedef bt::minimax<bt_rules>     bt_minimax;
typedef game::instance<bt_rules>  bt_game;

int main(int argc, char** argv){
    
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    try {
        
        // get command line inputs
        parser::commandline commp(argc,argv);

        bt::provided::defensive<NR,NC> defensive_h;
        bt::provided::offensive<NR,NC> offensive_h;

        bt_game game;
        bt_minimax p1, p2;
        p1.setMaxSearchDepth(2); p1.setUtilityEstimator(defensive_h);
        p2.setMaxSearchDepth(2); p2.setUtilityEstimator(offensive_h);
        game.addPlayer1(&p1);
        game.addPlayer2(&p2);

        game.play();
        bt_game::state_t gs = game.getFinalGameState();
        gs.print();


#ifdef CSP_TEST_SOLVE
        csp::test_csp tcsp;
        csp::test_csp::csp_state csp_;
        bool isSuccess = false;
        csp::test_csp::assignment a = tcsp.solve(csp_,isSuccess);

        for( auto it = a.begin(); it != a.end(); ++it ){
            printf("Assignment(%zu) = %i\n",it->first, (int)it->second);
        }
#endif
        
    
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
    printf("The code ran for %lf seconds\n", (time_span.count()/1000.0) );
    
	return 0;
}

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

// other helpful utility code
#include "FileWrap.hpp"

// game playing agent stuff
#include "breakthrough_rules.hpp"
#include "breakthrough_extended_rules.hpp"
#include "breakthrough_provided_heuristics.hpp"
#include "game_instance.hpp"
#include "breakthrough_minimax_agent.hpp"
#include "breakthrough_alphabeta_agent.hpp"
#include "breakthrough_utility_ab.hpp"
#include "breakthrough_cone_heuristics.hpp"

// optimization related stuff
#include "opt_pso.hpp"
#include "opt_test_rosenbrock2d.hpp"
#include "bt_heuristic_learning_costfunc.hpp"
#include "breakthrough_costfunc.hpp.hpp"

#define NR 8
#define NC 8

// game typedefs
typedef bt::baseline_rules<NR,NC> bt_rules;
typedef game::agent<bt_rules>     bt_agent;
typedef bt::minimax<bt_rules>     bt_minimax;
typedef bt::alphabeta<bt_rules>   bt_ab;
typedef bt::utility_ab<bt_rules>  bt_uab;
typedef game::instance<bt_rules>  bt_game;

// optimization typedefs
typedef opt::pso<bt::costfunc>       pso_t;
typedef opt::pso_iter<bt::costfunc>  cb_t;


int main(int argc, char** argv){
    
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    try {
        
        // get command line inputs
        parser::commandline commp(argc,argv);

#ifndef RUN_OPTIMIZATION

        int numParticles = 5, miters = 1;
        if( commp["-np"].size() != 0 ){
            numParticles = commp.convert<int>("-np");
        }
        if( commp["-miter"].size() != 0){
            miters = commp.convert<int>("-miter");
        }

        cb_t iter_cb;
        pso_t pso_solver;
        pso_solver.addCallback(&iter_cb);
        pso_solver.setNumParticles(numParticles);
        pso_solver.setMaxIterations(miters);
        pso_solver.setSearchBounds({0,0,0,0,0},{1,1,1,1,1});
        pso_solver.solve();

        // write optimal solution to file
        auto soln = pso_solver.getBestSolution();
        wrap::file optf("soln_8x8.txt",wrap::file::Write);
        if( optf.isOpen() ){
            fprintf(optf.ref(),"Best Cost: %lf\n",pso_solver.getBestCost());
            for(auto val: soln){
                fprintf(optf.ref(),"%lf, ",val);
            }
            fprintf(optf.ref(),"\n");
        }

#endif

#ifdef TEST_BREAKTHROUGH
        bt::provided::defensive<NR,NC> defensive_h;
        bt::provided::offensive<NR,NC> offensive_h;
        bt::cone::defensive<NR,NC>     dcone_h;
        bt::cone::offensive<NR,NC>     ocone_h;

        bt_game game;
        bt_minimax p1, p2;
        bt_ab p3,p4;
        bt_uab p5, p6;
        p1.setMaxSearchDepth(4); p1.setUtilityEstimator(defensive_h);
        p2.setMaxSearchDepth(4); p2.setUtilityEstimator(defensive_h);
        p3.setMaxSearchDepth(5); p3.setUtilityEstimator(defensive_h);
        p4.setMaxSearchDepth(4); p4.setUtilityEstimator(defensive_h);
        p5.setMaxSearchDepth(3); p5.setUtilityEstimator(defensive_h);
        p6.setMaxSearchDepth(3); p6.setUtilityEstimator(defensive_h);
        game.addPlayer1(&p6);
        game.addPlayer2(&p5);

        game.play();
        bt_game::state_t gs = game.getFinalGameState();

        // print avg move time for players
        printf("Avg move time for player 1 was: %lf ms\n",game.getAvgMoveTimeFor(0));
        printf("Avg move time for player 2 was: %lf ms\n",game.getAvgMoveTimeFor(1));

        // print avg expanded nodes per move for player
        printf("Avg expanded nodes per move for player 1 was: %lf nodes\n",game.getAvgNumberNodesExpandedPerMoveFor(0));
        printf("Avg expanded nodes per move for player 2 was: %lf nodes\n",game.getAvgNumberNodesExpandedPerMoveFor(1));

        // get number of moves made by each player
        printf("Number of moves to end of game by player 1 was %u\n",game.getNumMoves(0));
        printf("Number of moves to end of game by player 2 was %u\n",game.getNumMoves(1));

        // get total number of nodes expanded by each player
        printf("Total nodes expanded by player 1 is %u\n",game.getTotalNodesExpandedFor(0));
        printf("Total nodes expanded by player 2 is %u\n",game.getTotalNodesExpandedFor(1));

        // total pieces captured by players
        printf("Total pieces captured by player 1 is %u\n",game.getNumPiecesCapturedBy(bt::piece_t::Team1));
        printf("Total pieces captured by player 2 is %u\n",game.getNumPiecesCapturedBy(bt::piece_t::Team2));

        gs.print();
#endif


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

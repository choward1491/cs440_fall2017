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
#include <iostream>
#include <chrono>

// exception related stuff
#include "commandline_parser.hpp"
#include "MessageException.hpp"
#include "custom_exception.hpp"
#include "text_color.hpp"
#include "flow_io.h"
#include "test_csp.h"
#include "flow_csp.h"
#include "flow_solver.h"


int main(int argc, char** argv){
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    try {
        
        // get command line inputs
        parser::commandline commp(argc,argv);
        std::string flow_file   = commp["-flow"];
        std::string out_file   = commp["-out"];
        std::string flow_type   = commp["-ft"];


//#ifdef CSP_TEST_SOLVE
//        csp::test_csp tcsp;
//        csp::test_csp::csp_state csp_;
//        bool isSuccess = false;
//        csp::test_csp::assignment a = tcsp.solve(csp_,isSuccess);
//
//        for( auto it = a.begin(); it != a.end(); ++it ){
//            printf("Assignment(%zu) = %i\n",it->first, (int)it->second);
//        }
//#endif

        // setup flow free csp
//        csp::flow_csp fcsp;
//        csp::flow_csp::csp_state fcsp_;
//        bool beSmart = false;
//        
//        if(flow_type == "smart") beSmart = true;
//        
//        if( !flow_file.empty() ){
//            flow_io::loadFlow(flow_file, fcsp);
//        }else{
//            throw custom::exception("Did not pass in a flow file. This is the commandline argument `-flow`. Try again.");
//        }
//        
//        bool flowSolved = false;
//        csp::flow_csp::assignment fa = fcsp.solve(fcsp_,flowSolved);
//
//        for( auto it = fa.begin(); it != fa.end(); ++it ){
//            printf("Assignment(%zu) = %i\n",it->first, (int)it->second);
//        }
        
        // setup flow solver
        flow_solver fsolver;
        bool beSmart = false;
        if(flow_type == "smart") beSmart = true;
        fsolver.setSmart(beSmart);
        
        if( !flow_file.empty() ){
            fsolver.loadFlow(flow_file);
        }else{
            throw custom::exception("Did not pass in a flow file. This is the commandline argument `-flow`. Try again.");
        }
        
        // solve flow problem
        bool flowSolved = false;
        flowSolved = fsolver.solve();
        if(flowSolved) {
            std::cout << "Solved: " << std::endl;
            printf("attempts: %d\n",fsolver.getAttempts());
            fsolver.saveFlow(out_file);
        } else {
            std::cout << "flow could not be solved" << std::endl;
            printf("attempts: %d\n",fsolver.getAttempts());
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
    printf("The code ran for %lf seconds\n", (time_span.count()/1000.0) );
    
    return 0;
}

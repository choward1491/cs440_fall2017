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

// optimization related stuff
#include "opt_pso.hpp"
#include "opt_test_rosenbrock2d.hpp"
#include "bt_heuristic_learning_costfunc.hpp"
#include "breakthrough_costfunc.hpp.hpp"

// include testing related stuff and stuff to output data
#include "breakthrough_test_games.hpp"


#ifdef RUN_OPTIMIZATION
// optimization related stuff
#include "opt_pso.hpp"
#include "opt_test_rosenbrock2d.hpp"
#include "bt_heuristic_learning_costfunc.hpp"
#include "breakthrough_costfunc.hpp.hpp"

// optimization typedefs
typedef opt::pso<bt::costfunc>       pso_t;
typedef opt::pso_iter<bt::costfunc>  cb_t;
#endif


int main(int argc, char** argv){
    
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    try {
        
        // get command line inputs
        parser::commandline commp(argc,argv);

        printf("Starting 8x8 Nominal\n");
        bt::test::matchups_8x8nominal();

        printf("Starting 5x10 Nominal\n");
        bt::test::matchups_5x10nominal();

        printf("Starting 8x8 Extended\n");
        bt::test::matchups_8x8modified();

#ifdef RUN_OPTIMIZATION

        int numParticles = 5, miters = 1;
        if( commp["-np"].size() != 0 ){
            numParticles = commp.convert<int>("-np");
        }
        if( commp["-miter"].size() != 0){
            miters = commp.convert<int>("-miter");
        }

        cb_t iter_cb;
        pso_t pso_solver;
        int ndims = pso_solver.getCostFunction().numDims();
        pso_solver.addCallback(&iter_cb);
        pso_solver.setNumParticles(numParticles);
        pso_solver.setMaxIterations(miters);
        std::vector<double> lb(ndims,-100), ub(ndims,100);
        pso_solver.setSearchBounds(lb, ub);
        pso_solver.solve();

        // write optimal solution to file
        auto soln = pso_solver.getBestSolution();
        wrap::file optf("soln_coefs.txt",wrap::file::Write);
        if( optf.isOpen() ){
            fprintf(optf.ref(),"Best Cost: %lf\n",pso_solver.getBestCost());
            for(auto val: soln){
                fprintf(optf.ref(),"%lf, ",val);
            }
            fprintf(optf.ref(),"\n");
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

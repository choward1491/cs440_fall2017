//
// Created by Christian J Howard on 10/29/17.
//

#ifndef SRC_OPT_PSO_HPP
#define SRC_OPT_PSO_HPP

#include <cstdint>
#include <cmath>
#include <random>
#include <limits>
#include "opt_solver.hpp"
#include "text_color.hpp"

namespace opt {

    // forward declare pso iteration callback
    template<template<typename> typename costfunc> class pso_iter;


    /*
     * This code assumes you are solving minimization problems
     *
     */
    template< template<typename> typename costfunc >
    class pso : public solver<pso<costfunc>> {
    public:

        // useful typedefs
        typedef solver<pso<costfunc>>   parent;
        typedef typename parent::num_t  num_t;

        // constructor
        pso():nparticles(10),mc(0.8),lc(0.5),gc(0.3),
              seed(17),U(0.0,1.0),
              miter(100),niter(0){}

        // setters
        void setNumParticles( uint32_t np )         { nparticles = np; }
        void setMomentumCoefficient( num_t mom)     { mc = mom; }
        void setLocalCoefficient(num_t lcoef )      { lc = lcoef; }
        void setGlobalCoefficient( num_t gcoef )    { gc = gcoef; }
        void setSeed( uint32_t s )                  { seed = s; }
        void setMaxIterations( uint32_t miter_ )    { miter = miter_; }
        void setSearchBounds(   const std::vector<num_t> & lb_,
                                const std::vector<num_t> & ub_ )
        {
            lb = lb_; ub = ub_;
        }

        // getters
        const std::vector<num_t> & getBestSolution() { return gb; }
        const num_t              & getBestCost() { return gb_cost; }
        const std::vector<num_t> & getBestSolutionFor(uint32_t particle_num){ return pb[particle_num]; }
        const num_t              & getBestCostFor(uint32_t particle_num){ return pb_cost[particle_num]; }

        costfunc<num_t> & getCostFunction(){ return cfunc; }

    private:
        friend class pso_iter<costfunc>;
        friend solver<pso<costfunc>>;

        uint32_t nparticles, niter, miter, seed, ndim;
        std::default_random_engine rng;
        std::uniform_real_distribution<num_t> U;
        double mc, lc, gc;
        std::vector<std::vector<num_t>> p;  // particle positions
        std::vector<std::vector<num_t>> pb; // a particle's best position
        std::vector<num_t>              pb_cost; // personal best costs
        std::vector<num_t>              gb; // the global best position
        num_t                           gb_cost; // global best cost
        std::vector<std::vector<num_t>> v;  // particle velocities
        std::vector<num_t> lb; // lower bound of domain
        std::vector<num_t> ub; // upper bound of domain
        costfunc<num_t> cfunc;

        void init() {
            niter = 0;
            ndim = costfunc<num_t>::numDims();
            rng.seed(seed);
            randomInit();
        }

        bool isNotComplete() {
            return miter > niter;
        }

        void update() {

            // init temp variable for cost
            num_t tcost = 0.0;

            // loop through and update all particles
            // and their associated local/global info
            for(uint32_t n = 0; n < nparticles; ++n){
                updateParticle(n);
                tcost = cfunc(p[n]);

                if( tcost < pb_cost[n] ){
                    pb_cost[n]  = tcost;
                    pb[n].assign(p[n].begin(),p[n].end());
                    if( tcost < gb_cost ){
                        gb_cost = tcost;
                        gb.assign(p[n].begin(),p[n].end());
                    }// end if tcost is less than best global cost
                }// end if tcost is less than best local cost

            }// end for n

            // update iteration count
            niter++;
        }


        // initialize particles using Monte Carlo sampling
        void randomInit(){

            // init variables
            num_t s = 0.0, dv = 0.0;
            gb_cost = std::numeric_limits<num_t>::max();
            p.resize(nparticles);
            pb.resize(nparticles);
            pb_cost.resize(nparticles);
            v.resize(nparticles);

            // loop through and initialize all the particles
            // and their associated local/global data
            for(int n = 0; n < nparticles; ++n){
                p[n].resize(ndim);
                pb[n].resize(ndim);
                v[n].resize(ndim);

                // construct particle values and velocities via monte carlo
                for(int d = 0; d < ndim; ++d){
                    s           = U(rng);
                    p[n][d]     = lb[d]*(1-s) + ub[d]*s;
                    pb[n][d]    = p[n][d];

                    s           = U(rng);
                    dv          = std::abs(ub[d] - lb[d]);
                    v[n][d]     = dv*(1.0 - 2*s);
                }// end for d

                // evaluate particle positions to see what the
                // local and global best particles are
                pb_cost[n] = cfunc(pb[n]);
                if( pb_cost[n] < gb_cost ){
                    gb.assign(pb[n].begin(),pb[n].end());
                    gb_cost = pb_cost[n];
                }
            }// end for i
        }

        void updateParticle(uint32_t idx){
            num_t r1 = 0.0,r2 = 0.0;
            for(int d = 0; d < ndim; ++d){
                r1          = U(rng);
                r2          = U(rng);

                // update velocity via stochastic formula
                v[idx][d]   = mc*v[idx][d]
                              + lc*r1*(pb[idx][d]   - p[idx][d])
                              + gc*r2*(gb[d]        - p[idx][d]);

                // update position via velocity
                p[idx][d]    += v[idx][d];

                // do energy loss reflection if particle leaves some hyperrectangle boundary
                if      ( p[idx][d] > ub[d] ){ p[idx][d] = ub[d]; v[idx][d] *= -0.8; }
                else if ( p[idx][d] < lb[d] ){ p[idx][d] = lb[d]; v[idx][d] *= -0.8; }
            }// end for d
        }

    };


    // define iteration progress callback for PSO
    template<template<typename> typename costfunc>
    class pso_iter : public callback<pso<costfunc>> {
    public:
        typedef pso<costfunc> opt_t;

        // main callback method
        void run() {
            const opt_t* opt_ = this->getOptimizer();
            printf("Iteration(");
            text::printf_color(text::Yellow,"%6u",opt_->niter);
            printf(") | Best Cost so far is ");
            text::printf_color(text::Cyan,"%lf",static_cast<double>(opt_->gb_cost));
            printf("\n");
        }
    };

}

#endif //SRC_OPT_PSO_HPP

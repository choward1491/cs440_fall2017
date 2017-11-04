//
// Created by Christian J Howard on 10/29/17.
//

#ifndef SRC_OPT_SOLVER_HPP
#define SRC_OPT_SOLVER_HPP

#include <vector>
#include "opt_callbacks.hpp"

namespace opt {

#define CRTP(func) static_cast<impl*>(this)->func

    template<   typename impl,
                typename num_type = double>
    class solver {
    public:
        // useful typedefs
        typedef callback<impl>  callback_t;
        typedef num_type        num_t;

        solver()    = default;
        ~solver()   = default;

        void addCallback(callback_t* c) {
            c->setOptimizer(*static_cast<impl*>(this));
            callbacks.push_back(c);
        }

        void solve() {

            // initialize solver
            CRTP(init)();

            // main solving loop
            while( CRTP(isNotComplete)() ){

                // update solution
                CRTP(update)();

                // run callbacks
                for(callback_t* cb: callbacks ){ cb->run(); }

            }// end while not complete
        }

    protected:
        std::vector<callback_t*> callbacks;

    };

#undef CRTP(func)
}


#endif //SRC_OPT_SOLVER_HPP

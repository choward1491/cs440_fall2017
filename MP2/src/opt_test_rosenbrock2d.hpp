//
// Created by Christian J Howard on 10/29/17.
//

#ifndef SRC_OPT_TEST_ROSENBROCK2D_HPP
#define SRC_OPT_TEST_ROSENBROCK2D_HPP

#include <cstdint>
#include <vector>

namespace opt {
    namespace test {

        template<typename num_t>
        class rosenbrock2d {
        public:
            static uint32_t numDims() { return 2; }

            num_t operator()( const std::vector<num_t> & x){
                const num_t c = 100.0;
                const num_t dx= x[1] - x[0];
                const num_t d = x[0] - 1.0;
                return c*dx*dx + d*d;
            }

        };

    }
}

#endif //SRC_OPT_TEST_ROSENBROCK2D_HPP

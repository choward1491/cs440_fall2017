/*
 *  learning_rates.hpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/7/17
 *  Purpose:
 *
 */

#ifndef learning_rates_hpp
#define learning_rates_hpp

#include <stdlib.h>
#include <vector>

namespace RL {
    class constant_rate {
    public:
        constant_rate() = default;
        constant_rate(size_t,size_t);
        double operator()(unsigned int iter, size_t s, size_t a);
    };
    
    class inverse_decay_rate {
    public:
        inverse_decay_rate() = default;
        inverse_decay_rate(size_t,size_t);
        double operator()(unsigned int iter, size_t s, size_t a);
    };
    
    class state_tracker_rate {
    public:
        state_tracker_rate() = default;
        state_tracker_rate(size_t,size_t);
        double operator()(unsigned int iter, size_t s, size_t a);
    private:
        size_t ns, na;
        std::vector<unsigned int> N;
    };
}

#endif /* learning_rates_hpp */

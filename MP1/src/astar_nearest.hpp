/*
 *  astar_nearest.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/28/17
 *  Purpose:
 *
 */

#ifndef astar_nearest_hpp
#define astar_nearest_hpp

#include "astar_heuristic.hpp"
#include "multi_state.hpp"

namespace astar {
    
    // heuristic function for A* based on the manhatten distance
    class nearest : public heuristic_func_base<multi::state> {
    public:
        
        // ctor/dtor
        nearest();
        ~nearest() = default;
        
        // get name of heuristic
        std::string name() const;
        
        // distance measure
        double operator()( const multi::state & s ) const;
        
    private:
        bool hasInitialized;
        unsigned int current_goal;
    };
    
}

#endif /* astar_nearest_hpp */

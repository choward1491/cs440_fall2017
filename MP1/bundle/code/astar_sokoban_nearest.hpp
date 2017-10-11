/*
 *  astar_sokoban_nearest.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/30/17
 *  Purpose:
 *
 */

#ifndef astar_sokoban_nearest_hpp
#define astar_sokoban_nearest_hpp

#include "astar_heuristic.hpp"
#include "sokoban_state.hpp"

namespace sokoban {
    
        // heuristic function for A* based on the manhatten distance
    class nearest : public astar::heuristic_func_base<sokoban::state> {
        public:
            
            // ctor/dtor
            nearest();
            ~nearest() = default;
            
            // get name of heuristic
            std::string name() const;
            
            // distance measure
            double operator()( const sokoban::state & s ) const;
            
        private:
            
        };
    
}

#endif /* astar_sokoban_nearest_hpp */

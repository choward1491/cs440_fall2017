/*
 *  astar_euclidean.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/16/17
 *  Purpose:
 *
 */

#ifndef astar_euclidean_hpp
#define astar_euclidean_hpp

#include <string>
#include "astar_heuristic.hpp"
#include "multi_state.hpp"

namespace astar {
    
    // heuristic function for A* based on the manhatten distance
    class euclidean_dist : public heuristic_func_base<multi::state>{
    public:
        
        // ctor/dtor
        euclidean_dist() = default;
        ~euclidean_dist() = default;
        
        // get name of heuristic
        std::string name() const;
        
        // distance measure
        double operator()( const multi::state & s ) const;
        
    };
    
}

#endif /* astar_euclidean_hpp */

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

#include "astar_heuristic.hpp"

namespace astar {
    
    // heuristic function for A* based on the manhatten distance
    class euclidean_dist : public heuristic_func_base {
    public:
        
        // ctor/dtor
        euclidean_dist() = default;
        ~euclidean_dist() = default;
        
        // get name of heuristic
        std::string name() const;
        
        // distance measure
        unsigned int operator()( const multi::state & s ) const;
        
    };
    
}

#endif /* astar_euclidean_hpp */

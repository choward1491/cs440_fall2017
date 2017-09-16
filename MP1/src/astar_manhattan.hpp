/*
 *  astar_manhatten.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/16/17
 *  Purpose:
 *
 */

#ifndef astar_manhattan_hpp
#define astar_manhattan_hpp

#include "astar_heuristic.hpp"

namespace astar {
    
    // heuristic function for A* based on the manhatten distance
    class manhattan_dist : public heuristic_func_base {
    public:
        
        // ctor/dtor
        manhattan_dist() = default;
        ~manhattan_dist() = default;
        
        // get name of heuristic
        std::string name() const;
        
        // distance measure
        unsigned int operator()( maze::id_type node1 ) const;
        
    };
    
}

#endif /* astar_manhatten_hpp */

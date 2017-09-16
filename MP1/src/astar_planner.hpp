/*
 *  astar_planner.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/16/17
 *  Purpose:
 *
 */

#ifndef astar_planner_hpp
#define astar_planner_hpp

#include "path_planner_interface.hpp"
#include "astar_heuristic.hpp"

namespace astar {
    
    // This planning algorithm is going to  be a generic A* implementation that can
    // take any Heuristic Function, represented as a functor that satisfies a defined interface,
    // and use it to try and solve path planning problems.
    class planner : public path_planner {
    public:
        
        planner();
        ~planner() = default;
        
        // method to return the path planner name for potential use in
        // post processing or automated testing work
        std::string plannerName();
        
        // method(s) to be defined by specialized path planning algorithms.
        // this method should take an input maze reference and path reference and
        // generate a path that takes an agent from an initial point through all the
        // desired locations until there isn't any left. How optimal a path is
        // will be decided by the path planner algorithm being used.
        void computePath( const maze & maze_, path & path_ );
        
    private:
        
        
        
    };
    
}

#endif /* astar_planner_hpp */

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
#include <map>

namespace astar {
    
    // This planning algorithm is going to  be a generic A* implementation that can
    // take any Heuristic Function, represented as a functor that satisfies a defined interface,
    // and use it to try and solve path planning problems.
    template<typename transition_model>
    class planner : public path_planner {
    public:
        
        typedef typename transition_model::state_t state_t;
        
        //ctor/dtor
        planner();
        ~planner() = default;
        
        // method to return the path planner name for potential use in
        // post processing or automated testing work
        std::string plannerName() const;
        
        // method to perform A*, given some set heuristic function, to find
        // an efficient path through a given maze
        void computePath( const maze & maze_, path & path_ ) const;
        
        // method to set the heuristic function for the A* algorithm or greedy
        void setHeuristic( heuristic_func_base<state_t>& heuristic, bool beGreedy = false );
        
    private:
        heuristic_func_base<state_t>* h;
        
        
    };
    
}

#include "astar_planner_impl.hxx"

#endif /* astar_planner_hpp */

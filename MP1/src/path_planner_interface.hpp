/*
 *  path_planner.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/14/17
 *  Purpose:
 *
 */

#ifndef path_planner_hpp
#define path_planner_hpp

#include "maze.hpp"
#include "path.hpp"

class path_planner {
public:
    
    // ctor/dtor
    path_planner()          = default;
    virtual ~path_planner() = default;
    
    // method to return the path planner name for potential use in
    // post processing or automated testing work
    virtual std::string plannerName() const = 0;
    
    // method(s) to be defined by specialized path planning algorithms.
    // this method should take an input maze reference and path reference and
    // generate a path that takes an agent from an initial point through all the
    // desired locations until there isn't any left. How optimal a path is
    // will be decided by the path planner algorithm being used.
    virtual void computePath( const maze & maze_, path & path_ ) const = 0;
    
    
};

#endif /* path_planner_hpp */

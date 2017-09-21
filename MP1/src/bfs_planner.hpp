/*
 *  bfs_planner.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/16/17
 *  Purpose:
 *
 */

#ifndef bfs_planner_hpp
#define bfs_planner_hpp

#include "path_planner_interface.hpp"

namespace bfs {
class planner : public path_planner {
public:
    
    // ctor/dtor
    planner();
    ~planner(){}
    
    // method to return the path planner name for potential use in
    // post processing or automated testing work
    std::string plannerName() const;
    
    // method(s) to be defined by specialized path planning algorithms.
    // this method should take an input maze reference and path reference and
    // generate a path that takes an agent from an initial point through all the
    // desired locations until there isn't any left. How optimal a path is
    // will be decided by the path planner algorithm being used.
    void computePath( const maze & maze_, path & path_ ) const;
    
};
}

#endif /* bfs_planner_hpp */

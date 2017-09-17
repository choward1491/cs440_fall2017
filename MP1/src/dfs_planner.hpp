/*
 *  dfs_planner.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/17/17
 *  Purpose:
 *
 */

#ifndef dfs_planner_hpp
#define dfs_planner_hpp

#include "path_planner_interface.hpp"

namespace dfs {
    
    class planner : public path_planner {
    public:
        
        // ctor/dtor
        planner();
        ~planner(){}
        
        // method to return the path planner name for potential use in
        // post processing or automated testing work
        std::string plannerName() const;
        
        // method to perform a depth-first-search algorithm for
        // planning a path through the maze
        void computePath( const maze & maze_, path & path_ ) const;
        
        
    };

}



#endif /* dfs_planner_hpp */

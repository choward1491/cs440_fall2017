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

#include <cmath>
#include "maze.hpp"
#include "path.hpp"
#include <string>
#include <map>

class path_planner {
public:
    
    // internal absolute value
    static int abs( int value ){
        if( value < 0 ){ return -value; }
        return value;
    }
    
    // manhatten distance
    static unsigned int manhatten_dist( const maze::point & pt1, const maze::point & pt2 ){
        int pt1_[2] = { (int)pt1.first, (int)pt1.second };
        int pt2_[2] = { (int)pt2.first, (int)pt2.second };
        return abs(pt2_[0] - pt1_[0]) + abs(pt2_[1] - pt1_[1]);
    }
    
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
    
    
protected:
    
    void getResultingPathAndOutputData( const std::map<unsigned long,unsigned int> &  costFromStart,
                                       const std::map<unsigned long,unsigned long> & path_history,
                                       unsigned long start_state,
                                       unsigned long final_state,
                                       unsigned int num_goal_points,
                                       unsigned int num_maze_nodes,
                                       const maze & maze_,
                                       path & path_ ) const;
    
};

#endif /* path_planner_hpp */

/*
 *  path.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/14/17
 *  Purpose:
 *
 */

#ifndef path_hpp
#define path_hpp

#include "maze.hpp"

// structure that will store information about a path after being
// generated using some path planning algorithm
struct path {
    maze::maze_graph::node_list path_list;
    unsigned int                path_cost;
    unsigned int                num_nodes_expanded;
    void reset() {
        path_list.clear();
        path_cost = 0;
        num_nodes_expanded = 0;
    }
};

#endif /* path_hpp */
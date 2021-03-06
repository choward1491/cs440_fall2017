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
#include <stdio.h>
#include <string>

// structure that will store information about a path after being
// generated using some path planning algorithm
struct path {
    
    // internal path variables
    maze::maze_graph::node_list goal_visit_list;
    maze::maze_graph::node_list path_list;
    unsigned int                path_cost;
    unsigned int                num_nodes_expanded;
    
    // method to reset the path variables
    void reset() {
        goal_visit_list.clear();
        path_list.clear();
        path_cost = 0;
        num_nodes_expanded = 0;
    }
    
    void printResults(){
        printf("The path cost is %u after %u expanded nodes.\n",path_cost,num_nodes_expanded);
    }
    
    void writeResults(const std::string & filename){
        FILE* file = fopen(filename.c_str(),"w");
        if( file ){
            fprintf(file,"The path cost is %u after %u expanded nodes.\n",path_cost,num_nodes_expanded);
            fclose(file);
        }
    }
};

#endif /* path_hpp */

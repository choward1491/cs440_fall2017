/*
 *  maze_io.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/14/17
 *  Purpose:
 *
 */

#ifndef maze_reader_hpp
#define maze_reader_hpp

#include <stdio.h>
#include <string>
#include "maze.hpp"
#include "path.hpp"


namespace maze_io {
    
    // useful typedefs
    typedef maze::point         point;
    typedef maze::maze_graph    maze_graph;
    typedef maze::id_list       id_list;
    
    // main functions for loading and saving a graph
    void load_maze( const std::string & maze_file, maze & out_maze );
    void save_maze( const std::string & maze_file, const maze & in_maze, const path* nlist = nullptr );
}

#endif /* maze_reader_hpp */

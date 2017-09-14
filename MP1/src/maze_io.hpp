//
//  maze_reader.hpp
//  MP1
//
//  Created by Christian J Howard on 9/3/17.
//  Copyright © 2017 C. Howard. All rights reserved.
//

#ifndef maze_reader_hpp
#define maze_reader_hpp

#include <stdio.h>
#include <string>
#include "maze.hpp"


namespace maze_io {
    typedef maze::point         point;
    typedef maze::maze_graph    maze_graph;
    typedef maze::id_list       id_list;
    
    // main functions for loading and saving a graph
    void load_maze( const std::string & maze_file, maze & out_maze );
    void save_maze( const std::string & maze_file, const maze & in_maze, const maze_graph::node_list* nlist = nullptr );
}

#endif /* maze_reader_hpp */

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
    typedef std::pair<unsigned int, unsigned int>   point;
    typedef graph<point>                            maze_graph;
    typedef std::vector<maze_graph::id_type>        id_list;
    
    // useful hash and row transformation functions
    unsigned int hash(unsigned int row, unsigned int col, unsigned int num_cols);
    void getRowColFromHash( unsigned int hash, unsigned int num_cols, int & row, int & col );
    
    
    // main functions for loading and saving a graph
    void load_maze( const std::string & maze_file, maze & out_maze );
    void write_maze( const maze & in_maze, const maze_graph::node_list* nlist = nullptr );
}

#endif /* maze_reader_hpp */

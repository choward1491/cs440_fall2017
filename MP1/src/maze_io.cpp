//
//  maze_reader.cpp
//  MP1
//
//  Created by Christian J Howard on 9/3/17.
//  Copyright © 2017 C. Howard. All rights reserved.
//

#include "maze_io.hpp"
#include "FileWrap.hpp"
#include <queue>

namespace maze_io {
    /*
    typedef std::pair<unsigned int, unsigned int>   point;
    typedef graph<point>                            maze_graph;
     */
    
    unsigned int hash(unsigned int row, unsigned int col, unsigned int num_cols){
        return col + row*num_cols;
    }
    
    void getRowColFromHash( unsigned int hash, unsigned int num_cols, int & row, int & col ){
        col = static_cast<int>(hash%num_cols);
        row = static_cast<int>(hash/num_cols);
    }
    
    
    void load_maze( const std::string & maze_file,
                    maze_graph & out_graph,
                    id_list & final_points,
                    maze_graph::id_type & start_node_id )
    {
        
        // open maze file
        wrap::file maze(maze_file,wrap::file::Read);
        
        // process maze file if applicable
        if( maze.isOpen() ){
            
            // get number of columns for grid
            char c = '.';
            unsigned int rows = 1, cols = 0, idx = 0;
            while( fscanf(maze,"%c",&c) != EOF && c != '\n' ){ if( c == '%' ){++cols;} }
            
            // get number of rows for grid
            while( fscanf(maze,"%*[^\n]\n") != EOF ){ ++rows; }
            
            // allocate storage
            std::vector<bool> visit(rows*cols,false);
            std::vector<char> map(rows*cols,'%');
            out_graph.setNumNodes(rows*cols);
            
            // reset maze file and dump maze into map array
            maze.resetFile();
            while( fscanf(maze,"%c",&c) != EOF ){
                if( c == ' ' || c == '.' || c == 'P' || c == '%' ){
                    map[idx++] = c;
                }
            }
            
            // build graph based on the maze data
            int row = 0, col = 0;
            int dr[4] = {0, 1, 0, -1}, dc[4] = {-1, 0, 1, 0};
            bool isValidPos = false;
            final_points.clear();
            std::queue<maze_graph::id_type> bfs_list;
            bfs_list.push(0);
            
            
            // do breadth first search to populate graph nodes
            while( !bfs_list.empty() ){
                maze_graph::id_type id = bfs_list.front(); bfs_list.pop();
                if( visit[id] ){ continue; }
                
                // mark current node and do extra work if necessary
                switch( map[id] ){
                    case 'P': if( map[id] == 'P' ){ start_node_id = id; }
                    case '.': if( map[id] == '.' ){ final_points.push_back(id); }
                    case ' ': isValidPos = true; break;
                    case '%': isValidPos = false;
                    default: break;
                }
                
                // create edges in graph and add node data
                getRowColFromHash(id, cols, row, col);
                out_graph.addNode(id, point(row,col));
                for(int i = 0; i < 4; ++i){
                    int r = row + dr[i], c = col + dc[i];
                    
                    // if valid neighbor
                    if( (r >= 0 && r < rows) && (c >= 0 && c < cols) ){
                        unsigned int nid = hash(r,c,cols); // get id for neighbor node
                        
                        // if neighbor is same type as node(id), then connect them
                        char type = map[nid];
                        if( (type != '%') == isValidPos ){ out_graph.pair(id, nid); }
                        
                        // if neighbor hasn't been visited yet, add it to queue
                        if( !visit[nid] ){ bfs_list.push(nid); }
                    }
                }
                
                // mark node(id) as visited
                visit[id] = true;
                
            }// end while
        }// end if
    }
    void write_maze( const maze_graph & in_graph, const maze_graph::node_list* nlist ) {
        
    }
}

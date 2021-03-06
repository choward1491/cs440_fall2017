/*
 *  maze_io.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/14/17
 *  Purpose:
 *
 */

#include "maze_io.hpp"
#include "FileWrap.hpp"
#include "custom_exception.hpp"
#include <queue>
#include <map>

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
    
    
    void load_maze( const std::string & maze_file, maze & out_maze )
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
            out_maze.setDims(rows, cols);
            auto & out_graph = out_maze.getGraph();
            
            // reset maze file and dump maze into map array
            maze.resetFile();
            while( fscanf(maze,"%c",&c) != EOF ){
                if( c == ' ' || c == '.' || c == 'P' || c == '%' || c == 'b' || c == 'B' ){
                    map[idx++] = c;
                }
            }
            
            // build graph based on the maze data
            int row = 0, col = 0;
            int dr[4] = {0, 1, 0, -1}, dc[4] = {-1, 0, 1, 0};
            bool isValidPos = false;
            out_maze.clearGoalPoints();
            std::queue<maze_graph::id_type> bfs_list;
            bfs_list.push(0);
            
            
            // do breadth first search to populate graph nodes
            while( !bfs_list.empty() ){
                maze_graph::id_type id = bfs_list.front(); bfs_list.pop();
                if( visit[id] ){ continue; }
                
                // mark current node and do extra work if necessary
                switch( map[id] ){
                    case 'P': if( map[id] == 'P' ){ out_maze.setStartingLocationID(id); }
                    case 'B':
                    case '.': if( map[id] == '.' || map[id] == 'B' ){ out_maze.addGoalPoint(id); }
                    case 'b': if( map[id] == 'b' || map[id] == 'B' ){ out_maze.addBoxPosition(id); }
                    case ' ': isValidPos = true; break;
                    case '%': isValidPos = false;
                    default: break;
                }
                
                // create edges in graph and add node data
                maze::getRowColFromHash(id, cols, row, col);
                out_graph.addNode(id, point(row,col));
                out_maze.setValidityAtLocationID(id, isValidPos);
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
        else{
            throw custom::exception("No existing maze file has been passed into the executable. Revise your command line arguments and pass in a valid maze path using the `-maze` argument.");
        }
    }
    void save_maze( const std::string & maze_file, const maze & in_maze, const path * nlist ) {
        
        // open output maze file
        wrap::file mazeFile(maze_file,wrap::file::Write);
        
        // if output text file is open
        if( mazeFile.isOpen() ){
            
            // specify flag for if this is a Sokoban problem
            bool isSokoban = in_maze.numBoxes() > 0;
            
            // get reference to underlying graph
            const auto & mgraph = in_maze.getGraph();
            
            // allocate matrix to store characters from baseline graph
            unsigned int rows = in_maze.getNumRows(), cols = in_maze.getNumCols();
            std::vector<char> graph_mat(rows*cols);
            
            // loop through information from graph and populate char matrix
            for(unsigned int i = 0; i < mgraph.getNumNodes(); ++i){
                if( in_maze.getValidityAtLocationID(i) ){
                    if( in_maze.idIsGoalPoint(i) ){
                        graph_mat[i] = '.';
                        if( isSokoban ){graph_mat[i] = 'B';}
                    }
                    else if( in_maze.getStartingLocationID() == i ) { graph_mat[i] = 'P'; }
                    else{ graph_mat[i] = ' '; }
                }else{ graph_mat[i] = '%'; }
            }
            
            // add path to graph output if needed
            if( nlist ){
                
                // write out path to the maze
                for( auto & node : nlist->path_list ){
                    graph_mat[node] = '.';
                }
                graph_mat[in_maze.getStartingLocationID()] = 'P';
                
                // write out visited goal points in their appropriate order
                if( nlist->goal_visit_list.size() > 1){
                    std::map<unsigned int, unsigned int> visited_goals;
                    unsigned int idx = 0;
                    for( auto & node : nlist->goal_visit_list ){
                        char character = '.';
                        if( idx < 9 ){  character = '1' + idx; }
                        else{           character = 'a' + (idx - 9); }
                        
                        if( visited_goals.find(node) == visited_goals.end() ){
                            visited_goals[node] = node;
                            graph_mat[node] = character;
                            ++idx;
                        }
                    }// end for
                }// end if
            }
            
            // write the char matrix to file
            for(unsigned int ir = 0; ir < rows; ++ir ){
                for(unsigned int ic = 0; ic < cols; ++ic ){
                    fprintf(mazeFile,"%c",graph_mat[maze::hash(ir, ic, cols)]);
                }// end for ic
                fprintf(mazeFile,"\n"); // add next line
            }// end for ir
        }
    
    }
}

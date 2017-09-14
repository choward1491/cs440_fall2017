//
//  maze.cpp
//  MP1
//
//  Created by Christian J Howard on 9/5/17.
//  Copyright © 2017 C. Howard. All rights reserved.
//

#include "maze.hpp"

// internel state
/*
unsigned int rows, cols;
std::vector<id_type> goal_points;
maze_graph mgraph;
*/
    
// helpful typedefs
/*
typedef std::pair<unsigned int, unsigned int>   point;
typedef graph<point>                            maze_graph;
typedef std::vector<maze_graph::id_type>        id_list;
typedef maze_graph::id_type                     id_type;
*/

unsigned int maze::hash(unsigned int row, unsigned int col, unsigned int num_cols){
    return col + row*num_cols;
}

void maze::getRowColFromHash( unsigned int hash, unsigned int num_cols, int & row, int & col ){
    col = static_cast<int>(hash%num_cols);
    row = static_cast<int>(hash/num_cols);
}

//ctors/dtors
maze::maze():rows(0),cols(0) {

}

// getters/setters
void    maze::setStartingLocationID( id_type start_id ) {
    startID = start_id;
}
maze::id_type maze::getStartingLocationID( ) const {
    return startID;
}
void    maze::setValidityAtLocationID( id_type id, bool isValidNode ) {
    valid_node[id] = isValidNode;
}
bool    maze::getValidityAtLocationID( id_type id ) const {
    return valid_node[id];
}
void            maze::setDims( unsigned int num_rows, unsigned int num_cols ) {
    rows = num_rows;
    cols = num_cols;
    mgraph.setNumNodes(rows*cols);
    valid_node.resize(rows*cols);
}
unsigned int    maze::getNumRows() const {
    return rows;
}
unsigned int    maze::getNumCols() const {
    return cols;
}
maze::maze_graph & maze::getGraph() {
    return mgraph;
}
const   maze::maze_graph & maze::getGraph() const {
    return mgraph;
}
void            maze::addGoalPoint( id_type goal_id ) {
    goal_points.push_back(goal_id);
}
unsigned int    maze::numGoalPoints() const {
    return goal_points.size();
}
const maze::id_type & maze::goalPointAt(unsigned int idx) const {
    return goal_points[idx];
}
maze::id_type & maze::goalPointAt(unsigned int idx) {
    return goal_points[idx];
}
void            maze::clearGoalPoints() {
    goal_points.clear();
}

bool maze::idIsGoalPoint( id_type id ) const {
    for(unsigned int i = 0; i < goal_points.size(); ++i) {
        if( goal_points[i] == id ){ return true; }
    }
    return false;
}
    
    
    

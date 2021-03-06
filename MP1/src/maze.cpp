/*
 *  maze.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/14/17
 *  Purpose:
 *
 */

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
maze::point   maze::getCoordinateForID( id_type id) const {
    int row = 0, col = 0;
    getRowColFromHash(id, cols, row, col);
    return point(row,col);
}
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
    goal_points_map[goal_id] = goal_id;
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
const std::vector<maze::id_type> & maze::getGoalPoints() const {
    return goal_points;
}

const std::map<maze::id_type,maze::id_type> & maze::getGoalMap() const {
    return goal_points_map;
}

void            maze::clearGoalPoints() {
    goal_points.clear();
}

void maze::addBoxPosition( id_type box_id ) {
    boxes.push_back(box_id);
}
unsigned int maze::numBoxes() const {
    return boxes.size();
}
const maze::id_type & maze::positionForBox( unsigned int idx ) const {
    return boxes[idx];
}
maze::id_type & maze::positionForBox( unsigned int idx ) {
    return boxes[idx];
}
const std::vector<maze::id_type> & maze::getBoxPositions() const {
    return boxes;
}
void maze::clearBoxes() {
    boxes.clear();
}


maze::action_set maze::getActionSetForID( id_type id ) const {
    action_set aset;
    const auto & nlist = mgraph.getConnectivityListFor(id);
    const int icols = static_cast<int>(cols);
    for( auto&& node : nlist ){
        int del = static_cast<int>(node) - static_cast<int>(id);
        if      ( del == -1   )  { aset.push_back(Left);    }
        else if ( del == 1    )  { aset.push_back(Right);     }
        else if ( del == icols ) { aset.push_back(Down);     }
        else if ( del == -icols) { aset.push_back(Up);       }
        else                     { aset.push_back(Null);     }
    }
    
    return aset;
}

void maze::getActionSetForID( id_type id, action_set & aset ) const {

    maze::Action action = Null;
    unsigned int counter = 0;
    const auto & nlist = mgraph.getConnectivityListFor(id);
    const int icols = static_cast<int>(cols);
    aset.resize(0);
    for( auto&& node : nlist ){
        
        int del = static_cast<int>(node) - static_cast<int>(id);
        if      ( del == -1   )  { action = Left;    }
        else if ( del == 1    )  { action = Right;   }
        else if ( del == icols ) { action = Down;    }
        else if ( del == -icols) { action = Up;      }
        else                     { action = Null;    }
        aset.push_back(action);
    }
}

bool maze::idIsGoalPoint( id_type id ) const {
    for(unsigned int i = 0; i < goal_points.size(); ++i) {
        if( goal_points[i] == id ){ return true; }
    }
    return false;
}
    
    
    

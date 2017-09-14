//
//  maze.hpp
//  MP1
//
//  Created by Christian J Howard.
//  Copyright © 2017 C. Howard. All rights reserved.
//

#ifndef maze_hpp
#define maze_hpp

#include "graph.hpp"

/* maze class for representing useful maze information, such as a graph representation of the maze, starting location(s), and goal point location(s) for use by an intelligent agent
 */
class maze {
public:
    
    // helpful typedefs
    typedef std::pair<unsigned int, unsigned int>   point;
    typedef graph<point>                            maze_graph;
    typedef std::vector<maze_graph::id_type>        id_list;
    typedef maze_graph::id_type                     id_type;
    
    // helpful static methods
    static unsigned int hash(unsigned int row, unsigned int col, unsigned int num_cols);
    static void getRowColFromHash( unsigned int hash, unsigned int num_cols, int & row, int & col );
    
    //ctors/dtors
    maze();
    
    // getters/setters
    void    setStartingLocationID( id_type start_id );
    id_type getStartingLocationID() const;
    void    setValidityAtLocationID( id_type id, bool isValidNode );
    bool    getValidityAtLocationID( id_type id ) const;
    void            setDims( unsigned int num_rows, unsigned int num_cols );
    unsigned int    getNumRows() const;
    unsigned int    getNumCols() const;
            maze_graph & getGraph();
    const   maze_graph & getGraph() const;
    void            addGoalPoint( id_type goal_id );
    unsigned int    numGoalPoints() const;
    const id_type & goalPointAt(unsigned int idx) const;
          id_type & goalPointAt(unsigned int idx);
    void            clearGoalPoints();
    
    // some useful functionality
    bool idIsGoalPoint( id_type id ) const;
    
private:
    
    // internel state
    unsigned int rows, cols;
    id_type startID;
    std::vector<id_type> goal_points;
    std::vector<bool>    valid_node;
    maze_graph mgraph;
};

#endif /* maze_hpp */
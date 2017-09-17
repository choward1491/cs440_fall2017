/*
 *  astar_heuristic.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/16/17
 *  Purpose:
 *
 */

#ifndef astar_heuristic_hpp
#define astar_heuristic_hpp

#include "maze.hpp"
#include "multi_state.hpp"

namespace astar {
    
    // abstract base class to represent an A* heuristic function
    class heuristic_func_base {
    public:
        
        // ctor/dtor
        heuristic_func_base():maze_ref(nullptr),final_node(0){}
        virtual ~heuristic_func_base() = default;
        
        // get name of heuristic
        virtual std::string name() const = 0;
        
        // cost func
        virtual unsigned int operator()( const multi::state & s ) const = 0;
        
        // set reference to maze
        void setMaze( maze & maze_ ) { maze_ref = &maze_; }
        
        // set list of unvisited goal points
        void setUnvisitedGoalPointList( const std::vector<maze::id_type> & gplist){
            gplist_ref = &gplist;
        }
        
    protected:
        
        // methods for use in subclasses of this abstract heuristic function class
        const maze * getMaze() const { return maze_ref; }
        const std::vector<maze::id_type>* getGoalPointList() const { return gplist_ref; }
        maze::id_type getFinalNode() const { return final_node; }
        
    private:
        maze * maze_ref;
        const std::vector<maze::id_type>* gplist_ref;
        maze::id_type final_node;
    };
    
}

#endif /* astar_heuristic_hpp */

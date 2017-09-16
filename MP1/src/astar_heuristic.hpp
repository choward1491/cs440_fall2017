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

namespace astar {
    
    // abstract base class to represent an A* heuristic function
    class heuristic_func_base {
    public:
        
        // ctor/dtor
        heuristic_func_base():maze_ref(nullptr){}
        virtual ~heuristic_func_base() = default;
        
        // cost func
        virtual unsigned int operator()( maze::id_type node1, maze::id_type node2 ) const = 0;
        
        // set reference to maze
        void setMaze( maze & maze_ ) { maze_ref = &maze_; }
        
    protected:
        const maze * getMaze() const { return maze_ref; }
        
    private:
        maze * maze_ref;
    };
    
}

#endif /* astar_heuristic_hpp */

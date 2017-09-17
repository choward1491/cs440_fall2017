/*
 *  transition_model.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/16/17
 *  Purpose:
 *
 */

#ifndef transition_model_hpp
#define transition_model_hpp

#include "maze.hpp"
#include "multi_state.hpp"

namespace transition {
    
    // state transition model
    class model {
    public:
        
        // ctor/dtor
        model( const maze * mref = nullptr );
        ~model() = default;
        
        // define state transition operator
        multi::state operator()( const multi::state & current_state, maze::Action current_action );
        
        // method to set maze reference if needed
        void setMaze( const maze & maze);
        
        // set list of unvisited goal points
        void setUnvisitedGoalPointList( const std::vector<maze::id_type> & gplist);
        
    private:
        const maze* maze_ref;
        const std::vector<maze::id_type>* gplist_ref;
    };
    
}

#endif /* transition_model_hpp */

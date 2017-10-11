/*
 *  transition_model.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/16/17
 *  Purpose:
 *
 */

#ifndef maze_transition_model_hpp
#define maze_transition_model_hpp

#include "maze.hpp"
#include "multi_state.hpp"

namespace transition {
    
    // state transition model
    class maze_model {
    public:
        
        typedef multi::state state_t;
        
        // ctor/dtor
        maze_model( const maze * mref = nullptr );
        ~maze_model() = default;
        
        // define state transition operator
        multi::state operator()( const multi::state & current_state, maze::Action current_action );
        
        // method to set maze reference if needed
        void setMaze( const maze & maze);
        
        // set list of unvisited goal points
        void setGoalPointList( const std::vector<maze::id_type> & gplist);
        static void getActions( multi::state & x, const maze & maze_, std::vector<maze::Action> & action_set );
        
    private:
        const maze* maze_ref;
        const std::vector<maze::id_type>* gplist_ref;
    };
    
}

#endif /* transition_model_hpp */

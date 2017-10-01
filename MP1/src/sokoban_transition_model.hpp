/*
 *  sokoban_transition_model.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/26/17
 *  Purpose:
 *
 */

#ifndef sokoban_transition_model_hpp
#define sokoban_transition_model_hpp

#include "maze.hpp"
#include "sokoban_state.hpp"

namespace transition {
    
    // state transition model
    class sokoban_model {
    public:
        
        typedef sokoban::state state_t;
        
        // ctor/dtor
        sokoban_model( const maze * mref = nullptr );
        ~sokoban_model() = default;
        
        // define state transition operator
        sokoban::state operator()( const sokoban::state & current_state, maze::Action current_action );
        
        // method to set maze reference if needed
        void setMaze( const maze & maze);
        
        // set list of unvisited goal points
        void setGoalPointList( const std::vector<maze::id_type> & gplist );
        
        static maze::id_type newMazePos( maze::id_type, maze::Action current_action, const maze* mref );
        static void getActions( sokoban::state & x, const maze & maze_, std::vector<maze::Action> & action_set );
        
        
    private:
        const maze* maze_ref;
        const std::vector<maze::id_type>* gplist_ref;
    };
    
}

#endif /* sokoban_transition_model_hpp */

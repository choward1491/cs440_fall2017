/*
 *  transition_maze_model.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/16/17
 *  Purpose:
 *
 */

#include "maze_transition_model.hpp"
#include "custom_exception.hpp"

namespace transition {
    
    maze_model::maze_model( const maze * mref ):maze_ref(mref) {
        
    }
    
    multi::state maze_model::operator()( const multi::state & current_state, maze::Action current_action ) {
        
        multi::state new_state = current_state;
        
        if( maze_ref ){
            auto ncols = maze_ref->getNumCols();
            switch(current_action){
                case maze::Left:    new_state.current_node -= 1;     break;
                case maze::Right:   new_state.current_node += 1;     break;
                case maze::Up:      new_state.current_node -= ncols; break;
                case maze::Down:    new_state.current_node += ncols; break;
                case maze::Null:    break;
            }
            
            if( maze_ref->idIsGoalPoint(new_state.current_node) ){
                std::vector<bool> & goal_bools = new_state.hasSeenGoalPoint;
                for( unsigned int i = 0; i < goal_bools.size(); ++i){
                    if( !goal_bools[i] && new_state.current_node == (*gplist_ref)[i] ){
                        goal_bools[i] = true; break;
                    }
                }
            }
            
        }else{
            throw custom::exception("No maze has been assigned to the state transition, so can't properly obtain next state based on the current action.");
        }
        
        return new_state;
    }
    
    
    // method to set maze reference if needed
    void maze_model::setMaze( const maze & maze) {
        maze_ref = &maze;
    }
    
    
    void maze_model::setGoalPointList( const std::vector<maze::id_type> & gplist){
        gplist_ref = &gplist;
    }
    
    void maze_model::getActions( multi::state & x, const maze & maze_, std::vector<maze::Action> & action_set ) {
        maze_.getActionSetForID(x.current_node, action_set);
    }
    
}

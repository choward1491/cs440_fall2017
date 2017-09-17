/*
 *  transition_model.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/16/17
 *  Purpose:
 *
 */

#include "transition_model.hpp"
#include "custom_exception.hpp"

namespace transition {
    
    model::model( const maze * mref ):maze_ref(mref) {
        
    }
    
    multi::state model::operator()( const multi::state & current_state, maze::Action current_action ) {
        
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
            
            int i = 0;
            for(auto it = new_state.hasSeenGoalPoint.begin();
                it != new_state.hasSeenGoalPoint.end();
                ++it,++i )
            {
                if( !(*it) ){
                    if( new_state.current_node == (*gplist_ref)[i] ){
                        *it = true;
                        break;
                    }
                }
            }
            
        }else{
            throw custom::exception("No maze has been assigned to the state transition, so can't properly obtain next state based on the current action.");
        }
        
        return new_state;
    }
    
    
    // method to set maze reference if needed
    void model::setMaze( const maze & maze) {
        maze_ref = &maze;
    }
    
    
    void model::setUnvisitedGoalPointList( const std::vector<maze::id_type> & gplist){
        gplist_ref = &gplist;
    }
    
}

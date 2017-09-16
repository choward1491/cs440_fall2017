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
    
    maze::id_type model::operator()( maze::id_type current_state, maze::Actions current_action ) {
        
        maze::id_type new_state = current_state;
        
        if( maze_ref ){
            auto ncols = maze_ref->getNumCols();
            switch(current_action){
                case maze::Left:    new_state -= 1;
                case maze::Right:   new_state += 1;
                case maze::Up:      new_state -= ncols;
                case maze::Down:    new_state += ncols;
                case maze::Null:    break;
            }
        }else{
            throw custom::exception("No maze has been assigned to the state transition, so can't properly obtain next state based on the current action.");
        }
        
        return new_state;
    }
    
}

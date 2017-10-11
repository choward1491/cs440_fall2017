/*
 *  sokoban_transition_model.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/26/17
 *  Purpose:
 *
 */

#include "sokoban_transition_model.hpp"
#include "custom_exception.hpp"

namespace transition {
    
    // state transition model
        
    // ctor/dtor
    sokoban_model::sokoban_model( const maze * mref):maze_ref(mref),gplist_ref(nullptr) {
        
    }
    
    maze::id_type sokoban_model::newMazePos( maze::id_type current_node, maze::Action current_action, const maze* mref ) {
        maze::id_type new_node = current_node;
        auto ncols = mref->getNumCols();
        switch(current_action){
            case maze::Left:    new_node -= 1;     break;
            case maze::Right:   new_node += 1;     break;
            case maze::Up:      new_node -= ncols; break;
            case maze::Down:    new_node += ncols; break;
            case maze::Null:    break;
        }
        return new_node;
    }
    
    // define state transition operator
    sokoban::state sokoban_model::operator()( const sokoban::state & current_state, maze::Action current_action ){
        
        sokoban::state new_state = current_state;
        
        if( maze_ref ){
            
            new_state.current_node = newMazePos(current_state.current_node, current_action, maze_ref);
            for(auto it = new_state.box_locations.begin(); it != new_state.box_locations.end(); ++it){
                if( new_state.current_node == *it ){
                    *it = newMazePos( *it, current_action, maze_ref);
                    break;
                }
            }
            
            
        }else{
            throw custom::exception("No maze has been assigned to the state transition, so can't properly obtain next state based on the current action.");
        }
        
        return new_state;
    }
        
        // method to set maze reference if needed
    void sokoban_model::setMaze( const maze & maze) {
        maze_ref = &maze;
    }
        
    // set list of unvisited goal points
    void sokoban_model::setGoalPointList( const std::vector<maze::id_type> & gplist ) {
        gplist_ref = &gplist;
    }
    
    void sokoban_model::getActions( sokoban::state & x, const maze & maze_, std::vector<maze::Action> & action_set ) {
        action_set.resize(0);
        maze::Action possible_actions[4] = {maze::Up, maze::Right, maze::Down, maze::Left};
        maze::id_type p = x.current_node, pn = 0;
        const unsigned int numBoxes = x.box_locations.size();
        std::map<maze::id_type,maze::id_type> boxPos;
        
        for(unsigned int i = 0; i < numBoxes; ++i){
            boxPos[x.box_locations[i]] = 0;
        }
        
        for(unsigned int i = 0; i < 4; ++i){
            bool actionValid    = true;
            maze::Action action = possible_actions[i];
            pn = transition::sokoban_model::newMazePos(p, action, &maze_);
            
            if( maze_.getValidityAtLocationID(pn) ){
                if( boxPos.find(pn) != boxPos.end() ){
                    auto bn = transition::sokoban_model::newMazePos(pn, action, &maze_);
                    actionValid = maze_.getValidityAtLocationID(bn) && boxPos.find(bn) == boxPos.end();
                }
            }else{ continue; }
            
            if( actionValid ){ action_set.push_back(action); }
        }
    }
}

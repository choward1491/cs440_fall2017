/*
 *  sokoban_state.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/30/17
 *  Purpose:
 *
 */

#include "sokoban_state.hpp"

const std::map<maze::id_type,maze::id_type> * sokoban::state::gpos = nullptr;
void sokoban::state::setGoalLocationsMap( const std::map<maze::id_type,maze::id_type> & goal_locations ){
    gpos = &goal_locations;
}

const std::map<maze::id_type,maze::id_type> * sokoban::state::getGoalLocationsMap(){
    return gpos;
}

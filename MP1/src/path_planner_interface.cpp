/*
 *  path_planner_interface.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/20/17
 *  Purpose:
 *
 */

#include "path_planner_interface.hpp"
#include "multi_state.hpp"

void path_planner::getResultingPathAndOutputData( const std::map<unsigned long,unsigned int> &  costFromStart,
                                                 const std::map<unsigned long,unsigned long> & path_history,
                                                 unsigned long start_state,
                                                 unsigned long fstate,
                                                 unsigned int num_goal_points,
                                                 unsigned int num_maze_nodes,
                                                 const maze & maze_,
                                                 path & path_ ) const
{
    // define useful typedefs
    typedef multi::state mstate;
    
    // define state variables that will be used
    mstate final_state;
    final_state.current_node = (unsigned int)mstate::nodeFromStateIdx(fstate, num_maze_nodes);
    final_state.hasSeenGoalPoint.resize(num_goal_points,true);
    path_.path_cost         = costFromStart.find(fstate)->second;
    
    // get the resulting path going from the final node and working backwards
    unsigned long sidx = fstate;
    while( sidx != start_state ){
        auto node_id = static_cast<unsigned int>(mstate::nodeFromStateIdx(sidx, num_maze_nodes));
        if( maze_.idIsGoalPoint(node_id) ){ path_.goal_visit_list.push_front(node_id); }
        path_.path_list.push_front( node_id );
        sidx = path_history.find(sidx)->second;
    }
    
    // add final node for the starting point
    path_.path_list.push_front( static_cast<unsigned int>(mstate::nodeFromStateIdx(start_state, num_maze_nodes)) );
}

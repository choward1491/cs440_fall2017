/*
 *  multi_state.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/17/17
 *  Purpose:
 *
 */

#ifndef multi_state_hpp
#define multi_state_hpp

#include "maze.hpp"

namespace multi {
    
    // This state can be viewed as the following tuple:
    // state = (node_id, Boolean_1, Boolean_2, ... , Boolean_N )
    // where Boolean_i represents whether the agent has visited the ith Goal Point in the past
    struct state {
        
        // get the current node within the maze graph that the agent is in
        maze::id_type       current_node;
        
        // get the boolean tuple representing whether the AI agent has visited
        // a desired goal point
        std::vector<bool>   hasSeenGoalPoint;
        
        // method to see if the state has found a completed state
        bool isFinished() const {
            bool isDone = true;
            for(unsigned int i = 0; i < hasSeenGoalPoint.size(); ++i){
                isDone = isDone && hasSeenGoalPoint[i];
            }
            return isDone;
        }
        
        // define high dimensional index hashing and retrieval based on assumptions of state space
        static unsigned long highdim_hasher( const multi::state & s, unsigned long num_nodes ) {
            unsigned long idx = s.current_node;
            unsigned long factor = num_nodes;
            for(unsigned int i = 0; i < s.hasSeenGoalPoint.size(); ++i){
                idx += factor*s.hasSeenGoalPoint[i];
                factor *= 2;
            }
            return idx;
        }
        
        // method to get the node ID from a given state index hash value
        static unsigned long nodeFromStateIdx( unsigned long state_idx,
                                              unsigned long num_nodes )
        {
            return state_idx % num_nodes;
        }
        
        // method to compute the overall state space
        static unsigned long numOverallState( unsigned long num_goal_pts, unsigned long num_nodes ){
            unsigned long factor = num_nodes;
            for(unsigned int i = 0; i < num_goal_pts; ++i){ factor *= 2; }
            return factor;
        }
        
    };
}

#endif /* multi_state_hpp */

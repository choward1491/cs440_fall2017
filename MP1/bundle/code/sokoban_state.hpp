/*
 *  sokoban_state.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/26/17
 *  Purpose:
 *
 */

#ifndef sokoban_state_hpp
#define sokoban_state_hpp

#include "maze.hpp"
#include <map>

namespace sokoban {
    
    // This state can be viewed as the following tuple:
    // state = (node_id, Boolean_1, Boolean_2, ... , Boolean_N )
    // where Boolean_i represents whether the agent has visited the ith Goal Point in the past
    struct state {
        
        // get the current node within the maze graph that the agent is in
        maze::id_type       current_node;
        
        // box locations
        std::vector<maze::id_type> box_locations;
        
        // goal map of locations
        const maze* mref;
        
        // init state with a maze
        void initWithMaze( const maze & maze_ ){
            box_locations = maze_.getBoxPositions();
            mref = &maze_;
        }
        
        // method to see if the state has found a completed state
        bool isFinished() const {
            const auto & goal_locations = mref->getGoalMap();
            bool isDone = true;
            for(auto it = box_locations.begin(); it != box_locations.end(); ++it){
                isDone = isDone && goal_locations.find(*it) != goal_locations.end();
            }
            return isDone;
        }
        
        // set map to goal locations
        static const std::map<maze::id_type,maze::id_type> * gpos;
        static void setGoalLocationsMap( const std::map<maze::id_type,maze::id_type> & goal_locations );
        static const std::map<maze::id_type,maze::id_type> * getGoalLocationsMap();
        
        // define high dimensional index hashing and retrieval based on assumptions of state space
        static unsigned long highdim_hasher( const sokoban::state & s, unsigned long num_nodes ) {
            unsigned long idx = s.current_node;
            unsigned long factor = num_nodes;
            for(unsigned int i = 0; i < s.box_locations.size(); ++i){
                idx += factor*s.box_locations[i];
                factor *= num_nodes;
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
            for(unsigned int i = 0; i < num_goal_pts; ++i){ factor *= num_nodes; }
            return factor;
        }
        
    };
}

#endif /* sokoban_state_hpp */

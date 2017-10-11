/*
 *  bfs_planner.cpp
 *
 *  Project: MP1
 *  Author : Liuyi Shi
 *  Date   : 9/16/17
 *  Purpose:
 *
 */

#ifndef _bfs_planner_impl_
#define _bfs_planner_impl_

#include "bfs_planner.hpp"
#include "custom_exception.hpp"
#include <queue>

#ifdef _WIN32
#include <functional>
#endif

#define TEMPLATE_HEADER template<typename transition_model>
#define PLANNER planner<transition_model>

// useful typedefs
//typedef sokoban::state state_t;
//typedef std::pair<unsigned int, state_t> heap_node;

namespace bfs {
    
    // class definitions
    TEMPLATE_HEADER
    PLANNER::planner(){}
    
    TEMPLATE_HEADER
    std::string PLANNER::plannerName() const {
        return "bfs";
    }
    
    TEMPLATE_HEADER
    void PLANNER::computePath( const maze & maze_, path & path_ ) const {
        // define map of goal points
        const std::vector<maze::id_type> & goal_points = maze_.getGoalPoints();
        
        // define the transition model
        transition_model F;
        F.setMaze(maze_);
        F.setGoalPointList(goal_points);
        
        // define fifo that will be used in traversal
        std::queue<state_t> fifo;
        
        // define state variable
        state_t current_state;
        current_state.initWithMaze(maze_);
        
        // define maps that store whether a state has been visited, the cost to that state,
        // and the previous state that lead to a current state
        std::unordered_map<unsigned long,bool>           visited;
        std::unordered_map<unsigned int ,bool>           visited_goal;
        std::unordered_map<unsigned long,unsigned int>   costFromStart;
        std::unordered_map<unsigned long,unsigned long>  path_history;
        
        // define the number of nodes in the maze graph and the number of total states
        // after adding in the boolean chunk of the state space (based on number of goal points)
        auto num_nodes = maze_.getGraph().getNumNodes();
        auto nn        = num_nodes;
        auto num_states= state_t::numOverallState(goal_points.size(), num_nodes);
        
        // get the starting position, push into fifo
        maze::id_type sid           = maze_.getStartingLocationID();
        current_state.current_node  = sid;
        unsigned long sstate        = state_t::highdim_hasher(current_state,nn);
        unsigned long fstate        = 0;
        costFromStart[sstate]       = 0;
        fifo.push(current_state);
        
        // start main loop for traversal
        std::vector<maze::Action> action_list(5,maze::Null);
        path_.reset();
        while( !fifo.empty() ){
            
            // pop next node in fifo data structure
            auto state                      = fifo.front(); fifo.pop();
            unsigned long current_state_idx = state_t::highdim_hasher(state, nn);
            visited[current_state_idx]      = true;
            path_.num_nodes_expanded++;
            
            // check if current node is the final node, exit loop if it is
            if( state.isFinished() ){ fstate = current_state_idx; break; }
            
            // get list of actions for current node
            transition_model::getActions( state, maze_, action_list );
            
            // loop through actions and compute necessary data for use in search
            for( maze::Action a : action_list ){
                
                // compute state transition based on action
                auto new_state              = F(state,a);
                unsigned long new_state_idx = state_t::highdim_hasher(new_state, nn);
                
                // compute cost from start to new_node
                unsigned int newCostFromStart = costFromStart[current_state_idx] + 1;
                
                // update information for new_node and add to fifo if necessary
                if( visited.find(new_state_idx) == visited.end())
                {
                    path_history[new_state_idx]  = current_state_idx;
                    costFromStart[new_state_idx] = newCostFromStart;
                    fifo.push( new_state );
                }// end if
                
            }// end for
        }// end while
        
        // set the final path and output information
        this->getResultingPathAndOutputData( costFromStart, path_history, sstate, fstate,
                                      maze_.numGoalPoints(),num_nodes, maze_, path_ );
    }
}

#undef TEMPLATE_HEADER
#undef PLANNER

#endif

/*
 *  dfs_planner.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/17/17
 *  Purpose:
 *
 */

#include "dfs_planner.hpp"
#include "transition_model.hpp"
#include <stack>

namespace dfs {
    
// ctor/dtor
planner::planner(){
    
}

// method to return the path planner name for potential use in
// post processing or automated testing work
std::string planner::plannerName() const { return "DFS"; }

// method(s) to be defined by specialized path planning algorithms.
// this method should take an input maze reference and path reference and
// generate a path that takes an agent from an initial point through all the
// desired locations until there isn't any left. How optimal a path is
// will be decided by the path planner algorithm being used.
void planner::computePath( const maze & maze_, path & path_ ) const {
    /*if( maze_.numGoalPoints() == 1 ){
        
        // define the transition model
        transition::model F;
        F.setMaze(maze_);
        
        // initialize visited list, cost from start, and overall current cost estimate for given node
        // also initialize vector saying what the last node prior to arriving at node_i
        auto num_nodes = maze_.getGraph().getNumNodes();
        std::vector<bool>           visited(num_nodes, false);
        std::vector<unsigned int>   costFromStart(num_nodes,UINT32_MAX);
        std::vector<unsigned int>   path_history(num_nodes,0);
        
        // get the starting position and final position. Set cost at initial position to 0
        maze::id_type sid = maze_.getStartingLocationID();
        maze::id_type fid = maze_.goalPointAt(0);
        costFromStart[sid] = 0;
        
        // define stack and push first node into it
        std::stack<maze::id_type> traversal_stack;
        traversal_stack.push(sid);
        
        // do loop evaluating path
        std::vector<maze::Action> action_list(5,maze::Null);
        path_.reset();
        while( !traversal_stack.empty() ){
            
            // get node on top of stack to process
            auto node = traversal_stack.top(); traversal_stack.pop();
            
            // set information related to current node
            visited[node]   = true;
            path_.num_nodes_expanded++;
            
            // check if current node is the final node, exit loop if it is
            if( node == fid ){ break; }
            
            // get list of actions for current node
            maze_.getActionSetForID(node, action_list);
            
            // loop through actions and compute necessary data for use in search
            for( maze::Action a : action_list ){
                
                // compute state transition based on action
                auto new_node = F(node,a);
                
                // compute cost from start to new_node
                unsigned int newCostFromStart = costFromStart[node] + 1;
                
                // update information for new_node and add to traversal stack if necessary
                if( !visited[new_node]
                   ||
                   (visited[new_node] && newCostFromStart < costFromStart[new_node]) )
                {
                    path_history[new_node]  = node;
                    costFromStart[new_node] = newCostFromStart;
                    traversal_stack.push(new_node);
                }// end if
            }// end for
            
        }// end while
        
        // add cost for overall path
        path_.path_cost = costFromStart[fid];
        
        // get the resulting path going from start node to final node
        unsigned int id = fid;
        while( id != sid ){
            path_.path_list.push_front(id);
            id = path_history[id];
        }
        path_.path_list.push_front(sid);
        
    }*/
}

}
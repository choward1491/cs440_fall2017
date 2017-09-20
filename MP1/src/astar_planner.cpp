/*
 *  astar_planner.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/16/17
 *  Purpose:
 *
 */

#include "astar_planner.hpp"
#include "custom_exception.hpp"
#include "transition_model.hpp"
#include <queue>
#include <iostream>

// useful typedefs
typedef multi::state state_t;
typedef std::pair<unsigned int, state_t> heap_node;

// define std::greater for heap_node
namespace std {
    template<>
    struct greater<heap_node> {
        bool operator()( const heap_node& lhs, const heap_node& rhs) const {
            return lhs.first > rhs.first;
        }
    };
}


namespace astar {
    
    // define useful typedefs
    typedef std::priority_queue<heap_node, std::vector<heap_node>, std::greater<heap_node>> min_heap;
    
    // class definitions
    bool isGreedy;
    planner::planner():h(nullptr){}
    
    std::string planner::plannerName() const {
        if( h ){ return "A* - " + h->name(); }
        return "A*";
    }
    
    void planner::computePath( const maze & maze_, path & path_ ) const {
        
        // compute path, given heuristic is defined
        if( h ){
            
            // define map of goal points
            std::vector<maze::id_type> goal_points = maze_.getGoalPoints();
            
            // define the transition model
            transition::model F;
            F.setMaze(maze_);
            F.setUnvisitedGoalPointList(goal_points);
            
            // define min heap that will be used in traversal
            min_heap traversal_heap;
            
            // define state variable
            multi::state current_state;
            current_state.hasSeenGoalPoint.resize(goal_points.size(),false);
            
            // define maps that store whether a state has been visited, the cost to that state,
            // and the previous state that lead to a current state
            std::map<unsigned long,bool>           visited;
            std::map<unsigned int ,bool>           visited_goal;
            std::map<unsigned long,unsigned int>   costFromStart;
            std::map<unsigned long,unsigned long>  path_history;
            
            // define the number of nodes in the maze graph and the number of total states
            // after adding in the boolean chunk of the state space (based on number of goal points)
            auto num_nodes = maze_.getGraph().getNumNodes();
            auto nn        = num_nodes;
            auto num_states= multi::state::numOverallState(goal_points.size(), num_nodes);
            
            // get the starting position, set goal point list for heuristics, and throw into a min heap
            maze::id_type sid = maze_.getStartingLocationID();
            current_state.current_node = sid;
            unsigned long sstate = multi::state::highdim_hasher(current_state,nn);
            unsigned long fstate = 0;
            h->setUnvisitedGoalPointList(goal_points);
            costFromStart[sstate] = 0;
            traversal_heap.push(heap_node(costFromStart[sstate] + (*h)(current_state), current_state));
            
            // start main loop for traversal
            std::vector<maze::Action> action_list(5,maze::Null);
            path_.reset();
            while( !traversal_heap.empty() ){
                
                // pop next heap_node in min_heap data structure
                auto hnode      = traversal_heap.top(); traversal_heap.pop();
                auto state      = hnode.second;
                unsigned long current_state_idx = multi::state::highdim_hasher(state, nn);
                visited[current_state_idx]   = true;
                path_.num_nodes_expanded++;
                
                // check if current node is the final node, exit loop if it is
                if( state.isFinished() ){ fstate = current_state_idx; break; }
                
                // get list of actions for current node
                maze_.getActionSetForID(state.current_node, action_list);
                
                // loop through actions and compute necessary data for use in search
                for( maze::Action a : action_list ){
                    
                    // compute state transition based on action
                    auto new_state = F(state,a);
                    unsigned long new_state_idx = multi::state::highdim_hasher(new_state, nn);
                    
                    // compute cost from start to new_node
                    unsigned int newCostFromStart = costFromStart[current_state_idx] + 1;
                    
                    // update information for new_node and add to traversal heap if necessary
                    if( visited.find(new_state_idx) == visited.end()
                        ||
                        (visited[new_state_idx] && newCostFromStart < costFromStart[new_state_idx]) )
                    {
                        path_history[new_state_idx]  = current_state_idx;
                        costFromStart[new_state_idx] = newCostFromStart;
                        if(!isGreedy) {
                            traversal_heap.push(heap_node( newCostFromStart + (*h)(new_state) , new_state));
                        } else {
                            traversal_heap.push(heap_node( (*h)(new_state) , new_state));
                        }
                    }// end if
                    
                }// end for
            }// end while
            
            // set the final path and output information
            getResultingPathAndOutputData( costFromStart, path_history, sstate, fstate,
                                          maze_.numGoalPoints(),num_nodes, maze_, path_ );
            
            
        }else{
            throw custom::exception("No Heuristic Function has been provided to the A* algorithm, exitting.");
        }
    }
    
    void planner::setHeuristic( heuristic_func_base& heuristic) {
        h = &heuristic;
        isGreedy = false;
    }
    
    void planner::setHeuristic( heuristic_func_base& heuristic, bool beGreedy) {
        h = &heuristic;
        isGreedy = beGreedy;
    }
    
    void planner::getResultingPathAndOutputData( const std::map<unsigned long,unsigned int> &  costFromStart,
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
    
}

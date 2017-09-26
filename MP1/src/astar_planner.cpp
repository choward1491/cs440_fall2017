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

#ifdef _WIN32
#include <functional>
#endif

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
    
    template<>
    struct equal_to<heap_node> {
        bool operator()( const heap_node& lhs, const heap_node& rhs ) const {
            return lhs.first == rhs.first;
        }
    };
    
    template<>
    struct hash< heap_node >{
        typedef heap_node argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& s) const
        {
            result_type const h1 = 2098960;
            result_type const h2 = s.first;
            return h1 ^ (h2 << 1);
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
            const std::vector<maze::id_type> & goal_points = maze_.getGoalPoints();
            
            // define the transition model
            transition::model F;
            F.setMaze(maze_);
            F.setUnvisitedGoalPointList(goal_points);
            
            // define min heap that will be used in traversal
            min_heap frontier;
            
            // define state variable
            multi::state current_state;
            current_state.hasSeenGoalPoint.resize(goal_points.size(),false);
            
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
            auto num_states= multi::state::numOverallState(goal_points.size(), num_nodes);
            
            // get the starting position, set goal point list for heuristics, and throw into a min heap
            maze::id_type sid           = maze_.getStartingLocationID();
            current_state.current_node  = sid;
            unsigned long sstate        = multi::state::highdim_hasher(current_state,nn);
            unsigned long fstate        = 0;
            h->setUnvisitedGoalPointList(goal_points);
            costFromStart[sstate]       = 0;
            frontier.push(heap_node(costFromStart[sstate] + (*h)(current_state), current_state));
            
            // start main loop for traversal
            std::vector<maze::Action> action_list(5,maze::Null);
            path_.reset();
            while( !frontier.empty() ){
                
                // pop next heap_node in min_heap data structure
                auto hnode                      = frontier.top(); frontier.pop();
                auto state                      = hnode.second;
                unsigned long current_state_idx = multi::state::highdim_hasher(state, nn);
                visited[current_state_idx]      = true;
                path_.num_nodes_expanded++;
                
                // check if current node is the final node, exit loop if it is
                if( state.isFinished() ){ fstate = current_state_idx; break; }
                
                // get list of actions for current node
                maze_.getActionSetForID(state.current_node, action_list);
                
                // loop through actions and compute necessary data for use in search
                for( maze::Action a : action_list ){
                    
                    // compute state transition based on action
                    auto new_state              = F(state,a);
                    unsigned long new_state_idx = multi::state::highdim_hasher(new_state, nn);
                    
                    // compute cost from start to new_node
                    unsigned int newCostFromStart = costFromStart[current_state_idx] + 1;
                    
                    // update information for new_node and add to traversal heap if necessary
                    if( visited.find(new_state_idx) == visited.end()
                        ||
                        newCostFromStart < costFromStart[new_state_idx] )
                    {
                        path_history[new_state_idx]  = current_state_idx;
                        costFromStart[new_state_idx] = newCostFromStart;
                        auto heuristic_cost = (*h)(new_state);
                        if(!isGreedy) {
                            frontier.push(heap_node( newCostFromStart + heuristic_cost , new_state));
                        } else {
                            frontier.push(heap_node( heuristic_cost , new_state));
                        }
                    }// end if
                    
                }// end for
            }// end while
            
            // set the final path and output information
            this->getResultingPathAndOutputData( costFromStart, path_history, sstate, fstate,
                                          maze_.numGoalPoints(),num_nodes, maze_, path_ );
            
            
        }else{
            throw custom::exception("No Heuristic Function has been provided to the A* algorithm, exitting.");
        }
    }
    
    void planner::setHeuristic( heuristic_func_base& heuristic, bool beGreedy) {
        h = &heuristic;
        isGreedy = beGreedy;
    }
    
}

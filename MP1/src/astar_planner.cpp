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

// useful typedefs
typedef maze::id_type id_t;
typedef std::pair<unsigned int, id_t> heap_node;

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
    planner::planner():h(nullptr){}
    
    std::string planner::plannerName() const {
        if( h ){ return "A* - " + h->name(); }
        return "A*";
    }
    
    void planner::computePath( const maze & maze_, path & path_ ) const {
        
        if( h && maze_.numGoalPoints() == 1 ){
            
            // define the transition model
            transition::model F;
            F.setMaze(maze_);
            
            // define min heap that will be used in traversal
            min_heap traversal_heap;
            
            // initialize visited list, cost from start, and overall current cost estimate for given node
            // also initialize vector saying what the last node prior to arriving at node_i
            auto num_nodes = maze_.getGraph().getNumNodes();
            std::vector<bool>           visited(num_nodes, false);
            std::vector<unsigned int>   costFromStart(num_nodes,UINT32_MAX);
            std::vector<unsigned int>   path_history(num_nodes,0);
            
            // get the starting position and throw into a min heap
            maze::id_type sid = maze_.getStartingLocationID();
            maze::id_type fid = maze_.goalPointAt(0);
            h->setFinalNode(fid);
            costFromStart[sid] = 0;
            traversal_heap.push(heap_node(costFromStart[sid] + (*h)(sid), sid));
            
            // start main loop for traversal
            std::vector<maze::Action> action_list(5,maze::Null);
            path_.reset();
            while( !traversal_heap.empty() ){
                
                // pop next heap_node in min_heap data structure
                auto hnode      = traversal_heap.top(); traversal_heap.pop();
                auto node       = hnode.second;
                visited[node]   = true;
                path_.num_nodes_expanded++;
                
                // check if current node is the final node, exit loop if it is
                if( node == fid ){ break; }
                
                // get list of actions for current node
                maze_.getActionSetForID(hnode.second, action_list);
                
                // loop through actions and compute necessary data for use in search
                for( maze::Action a : action_list ){
                    
                    // compute state transition based on action
                    auto new_node = F(node,a);
                    
                    // compute cost from start to new_node
                    unsigned int newCostFromStart = costFromStart[node] + 1;
                    
                    // update information for new_node and add to traversal heap if necessary
                    if( !visited[new_node]
                        ||
                        (visited[new_node] && newCostFromStart < costFromStart[new_node]) )
                    {
                        path_history[new_node]  = node;
                        costFromStart[new_node] = newCostFromStart;
                        traversal_heap.push(heap_node( newCostFromStart + (*h)(new_node) ,new_node));
                    }// end if
                    
                }// end for
            }// end while
            
            // get the resulting path cost
            path_.path_cost = costFromStart[fid];
            
            // get the resulting path going from start node to final node
            unsigned int id = fid;
            while( id != sid ){
                path_.path_list.push_front(id);
                id = path_history[id];
            }
            path_.path_list.push_front(sid);
            
            
        }else{
            throw custom::exception("No Heuristic Function has been provided to the A* algorithm, exitting.");
        }
    }
    
    void planner::setHeuristic( heuristic_func_base& heuristic ) {
        h = &heuristic;
    }
    
}

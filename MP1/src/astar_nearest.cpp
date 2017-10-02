/*
 *  astar_nearest.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/28/17
 *  Purpose:
 *
 */

#include "astar_nearest.hpp"
#include "custom_exception.hpp"
#include <vector>
#include <cmath>

namespace astar {
    
    typedef std::vector<maze::id_type> gplist;
    
    inline int abs( int num ){
        return num < 0 ? -num : num ;
    }
    
    nearest::nearest(){
        
    }
    
    // get name of heuristic
    std::string nearest::name() const {
        return "Nearest Goal Heuristic";
    }
    
    double nearest::operator()( const multi::state & s ) const {
        
        // setup variables being used
        maze::id_type node1 = s.current_node;
        double cost         = UINT32_MAX;
        const maze* maze_   = this->getMaze();
        const gplist* gplist_= this->getGoalPointList();
        
        // compute manhattan distance
        if( maze_ && gplist_ ){
            
            // define temp vars and their values
            auto point1 = maze_->getCoordinateForID(node1);
            int idx     = 0;
            cost        = 1e100;
            int num_unvisited = 0;
            
            // loop through boolean part of state
            for( auto it = s.hasSeenGoalPoint.begin(); it != s.hasSeenGoalPoint.end(); ++it,++idx ){
                
                // if we have not visited ith Goal Point, compute distance to it from current node
                if( !(*it) ){
                    num_unvisited++;
                    auto point2 = maze_->getCoordinateForID( (*gplist_)[idx] );
                    auto local_cost =   abs( (int)point2.first - (int)point1.first ) +
                                        abs( (int)point2.second - (int)point1.second );
                    if( local_cost < cost ){ cost = local_cost; } // set cost as smallest distance seen thus far
                }
            }
            
            cost += 20.0*(num_unvisited-1);
            
        }else{ custom::exception("A* Nearest Goal Heuristic does not have a reference maze, cannot compute distance properly."); }
        return cost;
    }
    
}

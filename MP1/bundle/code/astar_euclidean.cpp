/*
 *  astar_euclidean.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/16/17
 *  Purpose:
 *
 */

#include <cmath>
#include <string>
#include "astar_euclidean.hpp"
#include "custom_exception.hpp"

namespace astar {

    typedef std::vector<maze::id_type> gplist;
    
    // get name of heuristic
    std::string euclidean_dist::name() const {
        return "Manhattan Distance";
    }

    double euclidean_dist::operator()( const multi::state & s ) const {
        
        // setup variables being used
        maze::id_type node1 = s.current_node;
        double cost   = UINT32_MAX;
        const maze* maze_   = this->getMaze();
        const gplist* gplist_= this->getGoalPointList();
        
        // compute euclidean distance
        if( maze_ && gplist_ ){
            
            // define temp vars and their values
            auto point1 = maze_->getCoordinateForID(node1);
            int idx     = 0;
            cost        = 0;
            
            // loop through boolean part of state
            for( auto it = s.hasSeenGoalPoint.begin(); it != s.hasSeenGoalPoint.end(); ++it,++idx ){
                
                // if we have not visited ith Goal Point, compute distance to it from current node
                if( !(*it) ){
                    auto point2 = maze_->getCoordinateForID( (*gplist_)[idx] );
                    double dx = (double)point2.first - (double)point1.first;
                    double dy = (double)point2.second - (double)point1.second;
                    cost += std::sqrt(dx*dx + dy*dy);
                }
            }
        }else{ custom::exception("A* Euclidean Distance Heuristic does not have a reference maze, cannot compute distance properly."); }
        return cost;
    }

}

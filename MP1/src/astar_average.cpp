/*
 *  astar_average.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/19/17
 *  Purpose:
 *
 */

#include "astar_average.hpp"
#include "custom_exception.hpp"
#include <vector>
#include <cmath>

namespace astar {
    
    typedef std::vector<maze::id_type> gplist;
    
    inline int abs( int num ){
        return num < 0 ? -num : num ;
    }
    
    average::average():scale_(1.0){
        
    }
    
    // get name of heuristic
    std::string average::name() const {
        return "Scaled Average Distance";
    }
    
    void average::setScaleFactor( double scale ) {
        scale_ = scale;
    }
    
    unsigned int average::operator()( const multi::state & s ) const {
        
        // setup variables being used
        maze::id_type node1 = s.current_node;
        unsigned int cost   = UINT32_MAX;
        const maze* maze_   = this->getMaze();
        const gplist* gplist_= this->getGoalPointList();
        double avg_dist     = 0.0;
        std::vector<double> distances;
        
        // compute manhattan distance
        if( maze_ && gplist_ ){
            
            // define temp vars and their values
            auto point1 = maze_->getCoordinateForID(node1);
            int idx     = 0;
            cost        = 0;
            int max_cost= 0;
            unsigned int num_unvisited = 0;
            
            // loop through boolean part of state
            for( auto it = s.hasSeenGoalPoint.begin(); it != s.hasSeenGoalPoint.end(); ++it,++idx ){
                
                // if we have not visited ith Goal Point, compute distance to it from current node
                if( !(*it) ){
                    auto point2 = maze_->getCoordinateForID( (*gplist_)[idx] );
                    auto local_cost = abs( (int)point2.first - (int)point1.first ) +
                    abs( (int)point2.second - (int)point1.second );
                    if( local_cost > max_cost ){ max_cost = local_cost; }
                    distances.push_back(static_cast<double>(local_cost));
                    num_unvisited++;
                }
            }
            
            // compute average distance and standard deviation
            double invN = 1.0 / static_cast<double>(distances.size());
            for( auto dist : distances ){ avg_dist += invN*dist; }
            cost = static_cast<unsigned int>( scale_*avg_dist );
            
            //cost = static_cast<unsigned int>( max_cost + scale_*(num_unvisited != 0)*( num_unvisited - 1) );
            //printf("Cost = %u, num_pts_left = %u, ",cost,num_unvisited);
            
            
        }else{ custom::exception("A* Scaled Average Distance Heuristic does not have a reference maze, cannot compute distance properly."); }
        return cost;
    }
    
}

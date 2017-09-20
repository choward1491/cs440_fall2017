/*
 *  astar_deviation.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/19/17
 *  Purpose:
 *
 */

#include "astar_deviation.hpp"
#include "custom_exception.hpp"
#include <vector>
#include <cmath>

namespace astar {
    
    typedef std::vector<maze::id_type> gplist;
    
    inline int abs( int num ){
        return num < 0 ? -num : num ;
    }
    
    deviation::deviation():scale_(1.0){
        
    }
    
    // get name of heuristic
    std::string deviation::name() const {
        return "Scaled Deviation Distance";
    }
    
    void deviation::setScaleFactor( double scale ) {
        scale_ = scale;
    }
    
    unsigned int deviation::operator()( const multi::state & s ) const {
        
        // setup variables being used
        maze::id_type node1 = s.current_node;
        unsigned int cost   = UINT32_MAX;
        const maze* maze_   = this->getMaze();
        const gplist* gplist_= this->getGoalPointList();
        double avg_dist     = 0.0;
        double std_dev_dist = 0.0;
        std::vector<double> distances;
        
        // compute manhattan distance
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
                    auto local_cost = abs( (int)point2.first - (int)point1.first ) +
                    abs( (int)point2.second - (int)point1.second );
                    distances.push_back(static_cast<double>(local_cost));
                }
            }
            
            // compute average distance and standard deviation
            double invN = 1.0 / static_cast<double>(distances.size());
            for( auto dist : distances ){
                avg_dist        += invN*dist;
                std_dev_dist    += invN*(dist*dist);
            }
            std_dev_dist -= (avg_dist*avg_dist);
            std_dev_dist = std::sqrt(std_dev_dist);
            
            cost = static_cast<unsigned int>( scale_*(avg_dist/invN + 3.0*std_dev_dist) );
            
            
        }else{ custom::exception("A* Scaled Deviation Distance Heuristic does not have a reference maze, cannot compute distance properly."); }
        return cost;
    }
    
}

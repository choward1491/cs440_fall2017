/*
 *  astar_blend.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 10/1/17
 *  Purpose:
 *
 */

#include "astar_blend.hpp"
#include "custom_exception.hpp"
#include <vector>
#include <cmath>

namespace astar {
    
    typedef std::vector<maze::id_type> gplist;
    
    inline int abs( int num ){
        return num < 0 ? -num : num ;
    }
    
    blend::blend(){
        
    }
    
    // get name of heuristic
    std::string blend::name() const {
        return "Blend Heuristic";
    }
    
    double beta(unsigned int num_visited, unsigned int total_goals ){
        const double ratio = static_cast<double>(num_visited)/static_cast<double>(total_goals);
        const double s = 1 - ratio;
        const double z = -10.0*s + 10.0*(1.0-s);
        const double er2   = exp(-z);
        //return exp(-1.0*delta*delta);
        return 1.0 / ( 1.0 + er2 );
    }
    
    double blend::operator()( const multi::state & s ) const {
        
        // setup variables being used
        maze::id_type node1 = s.current_node;
        double cost   = UINT32_MAX;
        const maze* maze_   = this->getMaze();
        const gplist* gplist_= this->getGoalPointList();
        std::vector<double> distances;
        
        // compute manhattan distance
        if( maze_ && gplist_ ){
            
            // define temp vars and their values
            auto point1 = maze_->getCoordinateForID(node1);
            int idx     = 0;
            cost        = 0;
            double max_cost    = 0;
            double total_cost  = 0;
            unsigned int num_unvisited  = 0;
            unsigned int num_total      = s.hasSeenGoalPoint.size();
            
            // loop through boolean part of state
            for( auto it = s.hasSeenGoalPoint.begin(); it != s.hasSeenGoalPoint.end(); ++it,++idx ){
                
                // if we have not visited ith Goal Point, compute distance to it from current node
                if( !(*it) ){
                    auto point2 = maze_->getCoordinateForID( (*gplist_)[idx] );
                    double local_cost =   abs( (int)point2.first - (int)point1.first ) +
                                        abs( (int)point2.second - (int)point1.second );
                    if( local_cost > max_cost ){ max_cost = local_cost; }
                    total_cost += local_cost;
                    num_unvisited++;
                }
            }
            
            double weight = beta(num_total - num_unvisited, num_unvisited);
            cost = (1.0 - weight)*total_cost + weight*max_cost;
            
        }else{ custom::exception("A* Blend Heuristic does not have a reference maze, cannot compute distance properly."); }
        return cost;
    }
    
}

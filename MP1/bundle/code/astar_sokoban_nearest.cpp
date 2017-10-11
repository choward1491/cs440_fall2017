/*
 *  astar_sokoban_nearest.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/30/17
 *  Purpose:
 *
 */

#include "astar_sokoban_nearest.hpp"
#include "custom_exception.hpp"
#include <vector>
#include <cmath>

namespace sokoban {
    
    typedef std::vector<maze::id_type> gplist;
    
    inline int abs( int num ){
        return num < 0 ? -num : num ;
    }
    
    nearest::nearest(){
        
    }
    
    // get name of heuristic
    std::string nearest::name() const {
        return "Sokoban Nearest Goal Heuristic";
    }
    
    double dist_between_pts( const maze::point & p1, const maze::point & p2, bool useL1 = true ) {
        // compute the euclidean distance between points
        double dx = (double)p2.first - (double)p1.first;
        double dy = (double)p2.second - (double)p1.second;
        
        if( useL1 ){ return abs(dx) + abs(dy); }
        return sqrt(dx*dx + dy*dy);
    }
    
    double nearest::operator()( const sokoban::state & s ) const {
        
        // setup variables being used
        maze::id_type node1 = s.current_node;
        double cost         = UINT32_MAX;
        const maze* maze_   = this->getMaze();
        const gplist* gplist_= this->getGoalPointList();
        
        // compute manhattan distance
        if( maze_ && gplist_ ){
            
            // define temp vars and their values
            auto point1 = maze_->getCoordinateForID(node1);
            cost        = 0;
            double min_p_b = 1e30, min_b_g = 1e30;
            
            // loop through boolean part of state
            const unsigned int numGoals = gplist_->size();
            const unsigned int numBoxes = s.box_locations.size();
            for(unsigned int i = 0; i < numBoxes; ++i){
                auto box = maze_->getCoordinateForID(s.box_locations[i]);
                double bdist = dist_between_pts(box, point1);
                min_p_b = min_p_b > bdist ? bdist : min_p_b;
                min_b_g = 1e30;
                for(unsigned int j = 0; j < numGoals; ++j){
                    auto goal = maze_->getCoordinateForID((*gplist_)[j]);
                    double dist = dist_between_pts(box,goal);
                    min_b_g = min_b_g > dist ? dist : min_b_g;
                }
                cost += min_b_g;
            }
            cost += min_p_b;
            
        }else{ custom::exception("A* Sokoban Nearest Goal Heuristic does not have a reference maze, cannot compute distance properly."); }
        return cost;
    }
}

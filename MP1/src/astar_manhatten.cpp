/*
 *  astar_manhatten.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/16/17
 *  Purpose:
 *
 */

#include "astar_manhatten.hpp"
#include "custom_exception.hpp"


namespace astar {
    
    inline int abs( int num ){
        return num < 0 ? -num : num ;
    }
    
    unsigned int manhatten_dist::operator()( maze::id_type node1, maze::id_type node2 ) const {
        unsigned int cost = UINT32_MAX;
        const maze* maze_ = this->getMaze();
        if( maze_ ){
            auto point1 = maze_->getCoordinateForID(node1);
            auto point2 = maze_->getCoordinateForID(node2);
            cost =  abs( (int)point2.first - (int)point1.first ) +
                    abs( (int)point2.second - (int)point1.second );
        }else{ custom::exception("A* Manhatten Distance Heuristic does not have a reference maze, cannot compute distance properly."); }
        return cost;
    }
    
}

/*
 *  astar_manhatten.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/16/17
 *  Purpose:
 *
 */

#include "astar_manhattan.hpp"
#include "custom_exception.hpp"


namespace astar {
    
    inline int abs( int num ){
        return num < 0 ? -num : num ;
    }
    
    // get name of heuristic
    std::string manhattan_dist::name() const {
        return "Manhattan Distance";
    }
    
    unsigned int manhattan_dist::operator()( maze::id_type node1 ) const {
        unsigned int cost = UINT32_MAX;
        const maze* maze_ = this->getMaze();
        if( maze_ ){
            auto point1 = maze_->getCoordinateForID(node1);
            auto point2 = maze_->getCoordinateForID(this->getFinalNode());
            cost =  abs( (int)point2.first - (int)point1.first ) +
                    abs( (int)point2.second - (int)point1.second );
        }else{ custom::exception("A* Manhattan Distance Heuristic does not have a reference maze, cannot compute distance properly."); }
        return cost;
    }
    
}

/*
 *  convex_hull.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 10/1/17
 *  Purpose:
 *
 */

#ifndef convex_hull_hpp
#define convex_hull_hpp

#include "maze.hpp"

namespace chull {
    double dist_between_pts( const maze::point & p1, const maze::point & p2, bool useL1 );
    double dist_to_line( const std::vector<maze::point> & point_cloud, const maze::point & edge, const maze::point & pt );
    void get_chull( const std::vector<maze::point> & point_cloud, std::vector<maze::point> & chull_edge_list );
}
    
#endif /* convex_hull_hpp */

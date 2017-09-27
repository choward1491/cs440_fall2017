/*
 *  astar_convexhull.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/25/17
 *  Purpose:
 *
 */

#ifndef astar_convexhull_hpp
#define astar_convexhull_hpp

#include "astar_heuristic.hpp"
#include "multi_state.hpp"

namespace astar {
    
    // heuristic function for A* based on the manhatten distance
    class convexhull : public heuristic_func_base<multi::state> {
    public:
        
        // ctor/dtor
        convexhull();
        ~convexhull() = default;
        
        // get name of heuristic
        std::string name() const;
        
        // distance measure
        unsigned int operator()( const multi::state & s ) const;
        
        // function for setting a scale factor for turning into sub-optimal heuristic
        void setScaleFactor( double scale );
        
    private:
        static int dist_between_pts( const maze::point & p1, const maze::point & p2 );
        static double dist_to_line( const std::vector<maze::point> & point_cloud, const maze::point & edge, const maze::point & pt );
        static void get_chull( const std::vector<maze::point> & point_cloud, std::vector<maze::point> & chull_edge_list );
        double scale_;
        
    };
    
}

#endif /* astar_convexhull_hpp */

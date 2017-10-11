/*
 *  astar_convexhull_fast.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/30/17
 *  Purpose:
 *
 */

#include "astar_convexhull_fast.hpp"
#include "custom_exception.hpp"
#include <vector>
#include <cmath>
#include <map>
#include "convex_hull.hpp"

typedef std::pair<double,unsigned int> chull_pair;

namespace std {
    
    template<>
    struct less<chull_pair> {
        bool operator()( const chull_pair& lhs, const chull_pair& rhs ) const {
            return lhs.first < rhs.first;
        }
    };
    
}


namespace astar {
    namespace fast {
    
    typedef std::vector<maze::id_type> gplist;
    
    inline int abs( int num ){
        return num < 0 ? -num : num ;
    }
    
    convexhull::convexhull():scale_(1.0){
        
    }
    
    // get name of heuristic
    std::string convexhull::name() const {
        return "Convex Hull Distance";
    }
    
    void convexhull::setScaleFactor( double scale ) {
        scale_ = scale;
    }
    
    double convexhull::dist_between_pts( const maze::point & p1, const maze::point & p2, bool useL1 ) {
        // compute the euclidean distance between points
        double dx = (double)p2.first - (double)p1.first;
        double dy = (double)p2.second - (double)p1.second;
        
        if( useL1 ){ return abs(dx) + abs(dy); }
        return sqrt(dx*dx + dy*dy);
    }
    
    double convexhull::dist_to_line( const std::vector<maze::point> & point_cloud, const maze::point & edge, const maze::point & pt ) {
        // compute the normal vector
        double dx[2] = { (double)point_cloud[edge.first].first - (double)point_cloud[edge.second].first, (double)point_cloud[edge.first].second - (double)point_cloud[edge.second].second };
        double mag = sqrt(dx[0]*dx[0] + dx[1]*dx[1]);
        double tv[2] = { dx[0]/mag, dx[1]/mag };
        double n[2] = { -tv[1], tv[0] };
        
        // compute dot product to get nearest distance from pt to the edge
        dx[0] = (double)pt.first - (double)point_cloud[edge.first].first;
        dx[1] = (double)pt.second - (double)point_cloud[edge.first].second;
        return n[0]*dx[0] + n[1]*dx[1];
    }
    
    
    double convexhull::operator()( const multi::state & s ) const {
        
        // setup variables being used
        maze::id_type node1 = s.current_node;
        double cost   = UINT32_MAX;
        const maze* maze_   = this->getMaze();
        const gplist* gplist_= this->getGoalPointList();
        std::vector<maze::point> unvisited_pts, chull_edges;
        std::vector<double> distances;
        
        // compute manhattan distance
        if( maze_ && gplist_ ){
            
            // define temp vars and their values
            auto point1 = maze_->getCoordinateForID(node1);
            int idx     = 0;
            cost        = 0;
            int min_cost= INT32_MAX;
            unsigned int num_unvisited = 0;
            
            // loop through boolean part of state
            for( auto it = s.hasSeenGoalPoint.begin(); it != s.hasSeenGoalPoint.end(); ++it,++idx ){
                
                // if we have not visited ith Goal Point, compute distance to it from current node
                if( !(*it) ){
                    auto point2 = maze_->getCoordinateForID( (*gplist_)[idx] );
                    auto local_cost = dist_between_pts(point1,point2);
                    //cost += 1e-3*local_cost;
                    if( local_cost < min_cost ){ min_cost = local_cost; }
                    unvisited_pts.push_back(point2);
                    num_unvisited++;
                }
            }
            
            // compute convex hull based distance measurement
            int chull_dist = 0;
            double avg_edge_len = 0.0;
            unsigned int len = unvisited_pts.size();
            unvisited_pts.push_back(point1);
            if( unvisited_pts.size() > 1 ){
                // compute convex hull around 2D point cloud
                chull::get_chull( unvisited_pts, chull_edges );
                
                // compute perimeter around convex hull
                const double inv_ne = 1.0/((double)(chull_edges.size()-1));
                const double inv_sq2 = 1.0 / sqrt(2.0);
                unsigned int max_dist = 0;
                double dist = 0;
                for(unsigned int i = 0; i < chull_edges.size(); ++i){
                    auto edge = chull_edges[i];
                    dist = dist_between_pts(unvisited_pts[edge.first], unvisited_pts[edge.second]);
                    if( /*(edge.first == len || edge.second == len ) &&*/ dist > max_dist ){ max_dist = dist; }
                    chull_dist += (int)dist;
                    avg_edge_len += (1.0*dist*inv_sq2*inv_ne);
                }
                
                // subtract largest edge distance since an agent would not go around the whole perimeter
                chull_dist -= max_dist;
                avg_edge_len -= (1.0*(max_dist+1)*inv_sq2*inv_ne);
            }
            
            // compute overall distance estimate based on convex hull and penalization of unvisited goals
            cost = (int)( chull_dist + 2.0*(num_unvisited - (chull_edges.size()-1)) + avg_edge_len*(chull_edges.size()-1) );
            
        }else{ custom::exception("A* Convex Hull Distance Heuristic does not have a reference maze, cannot compute distance properly."); }
        return cost;
    }
    
}
}

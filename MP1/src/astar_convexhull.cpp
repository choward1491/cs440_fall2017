/*
 *  astar_convexhull.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/25/17
 *  Purpose:
 *
 */

#include "astar_convexhull.hpp"
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
    
    typedef std::vector<maze::id_type> gplist;
    
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
            double chull_dist       = 0;
            double avg_edge_len     = 0.0;
            const double beta       = 0.01;
            unsigned int num_interior_goals = 0;
            unsigned int len = unvisited_pts.size();
            unsigned int pidx= len - 1;
            unvisited_pts.push_back(point1);
            std::vector<bool> on_hull(unvisited_pts.size(),false);
            
            if( unvisited_pts.size() > 1 ){
                // compute convex hull around 2D point cloud
                chull::get_chull( unvisited_pts, chull_edges );
                
                // compute perimeter around convex hull
                bool agent_on_hull = true;
                double dist = 0, max_dist1 = 0, max_dist2 = 0, min_dist = 1e30;
                
                for(unsigned int i = 0; i < chull_edges.size(); ++i){
                    
                    // get edge on convex hull
                    auto edge = chull_edges[i];
                    
                    // set that ith edge's first vertex is on the convex hull
                    on_hull[edge.first] = true;
                    
                    // compute weighted L1 and L2 distance between vertices on edge
                    dist =  beta*dist_between_pts(unvisited_pts[edge.first], unvisited_pts[edge.second]) +
                            (1.0 - beta)*dist_between_pts(unvisited_pts[edge.first], unvisited_pts[edge.second],true);
                    
                    // if the edge has the agent's position vertex, update max edge distance
                    // and set the flag to say the agent is on the convex hull
                    if( edge.first == pidx || edge.second == pidx ){ agent_on_hull = true; max_dist1 = dist > max_dist1 ? dist : max_dist1; }
                    
                    // else, compute the relative distance from the ith edge's first vertex
                    // to the agent's position and update the minimum distance to agent variable
                    else{
                        double rel_dist = beta*dist_between_pts(unvisited_pts[edge.first], unvisited_pts[pidx]) +
                                        (1.0 - beta)*dist_between_pts(unvisited_pts[edge.first], unvisited_pts[pidx],true);;
                        min_dist = min_dist > rel_dist ? rel_dist : min_dist;
                    }
                    
                    // compute maximum edge length within convex hull
                    max_dist2 = dist > max_dist2 ? dist : max_dist2;
                    
                    // update the convex hull with the current edge length
                    chull_dist += dist;
                }
                
                // if agent is on the convex hull, remove the largest edge distance the agent sits on
                if( agent_on_hull ) { chull_dist -= max_dist1;              }
                
                // else remove the largest edge on the convex hull and add the minimal distance
                // from the agent to a point on the convex hull
                else                { chull_dist += (min_dist - max_dist2); }
                
                // set the number of goals on the interior of the convex hull
                num_interior_goals = unvisited_pts.size() - (chull_edges.size() - agent_on_hull);
            }
            
            
            // compute minimum distance between interior points to chull point
            // and between interior points to other interior points
            bool setDistance = false;
            double min_interior_dist = 1e30;
            /*for(unsigned int i = 0; i < unvisited_pts.size(); ++i){
                if( !on_hull[i] ){
                    for(unsigned int j = 0; j < unvisited_pts.size(); ++j){
                        if( on_hull[j] ){
                            double rel_dist = beta*dist_between_pts(unvisited_pts[i], unvisited_pts[j]) +
                                            (1.0 - beta)*dist_between_pts(unvisited_pts[i], unvisited_pts[j],true);;
                            min_interior_dist = min_interior_dist > rel_dist ? rel_dist : min_interior_dist;
                            setDistance = true;
                        }else if( i != j ){
                            double rel_dist = beta*dist_between_pts(unvisited_pts[i], unvisited_pts[j]) +
                                            (1.0 - beta)*dist_between_pts(unvisited_pts[i], unvisited_pts[j],true);;
                            min_interior_dist = min_interior_dist > rel_dist ? rel_dist : min_interior_dist;
                            setDistance = true;
                        }
                    }// end for
                }// end if
            }// end for*/
            
            if( !setDistance ){ min_interior_dist = 1.0; }
            
            // compute overall distance estimate based on convex hull and penalization of unvisited goals
            cost = ( chull_dist + (scale_*min_interior_dist)*static_cast<double>(num_interior_goals) );
            
        }else{ custom::exception("A* Convex Hull Distance Heuristic does not have a reference maze, cannot compute distance properly."); }
        return cost;
    }
    
}

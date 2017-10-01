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
#include <queue>
#include <map>

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
    
    void convexhull::get_chull( const std::vector<maze::point> & point_cloud, std::vector<maze::point> & chull_edge_list ) {
        // define heap storage for quickly finding max point
        typedef std::priority_queue<chull_pair> max_heap;
        typedef std::pair<maze::point,max_heap> face_set;
        enum dim { X = 0, Y };
        
        // zero out size of chull edge list
        chull_edge_list.clear();
        
        // find the bounding faces
        int idx = 0, min_idx[2] = {0}, max_idx[2] = {0};
        double min_val[2] = {1e30, 1e30}, max_val[2] = {-1e30,-1e30};
        std::queue<unsigned int> pt_idx;
        for( const auto & pt : point_cloud ){
            pt_idx.push(idx);
            if( min_val[X] > pt.first ) { min_val[X] = pt.first; min_idx[X] = idx; }
            if( min_val[Y] > pt.second ){ min_val[Y] = pt.second; min_idx[Y] = idx; }
            if( max_val[X] < pt.first ) { max_val[X] = pt.first; max_idx[X] = idx; }
            if( max_val[Y] < pt.second ){ max_val[Y] = pt.second; max_idx[Y] = idx; }
            ++idx;
        }
        
        // define all the edges and find the maximum edge
        maze::point edges[6] = {    maze::point(min_idx[X],min_idx[Y]), maze::point(min_idx[X],max_idx[Y]),
                                    maze::point(min_idx[X],max_idx[X]), maze::point(min_idx[Y],max_idx[Y]),
                                    maze::point(min_idx[Y],max_idx[X]), maze::point(max_idx[Y],max_idx[X])};
        idx = 0; max_val[0] = 0.0;
        for(unsigned int i = 0; i < 6; ++i ){
            double dx   = (double)point_cloud[edges[i].first].first - (double)point_cloud[edges[i].second].first;
            double dy   = (double)point_cloud[edges[i].first].second - (double)point_cloud[edges[i].second].second;
            double dist = dx*dx + dy*dy;
            if( max_val[0] < dist ){ max_val[0] = dist; idx = i; }
        }
        
        // get the max edge and find points in front of the two faces it generates
        maze::point edge1 = edges[idx], edge2 = maze::point(edge1.second,edge1.first);
        face_set f1, f2;
        f1.first = edge1; f2.first = edge2;
        
        // add points to first face
        for(unsigned int i = 0; i < point_cloud.size(); ++i){
            unsigned int id = pt_idx.front(); pt_idx.pop();
            double dist = dist_to_line(point_cloud, edge1, point_cloud[id]);
            if( dist > 0.0 ){
                f1.second.push( chull_pair(dist, id) );
            }else{
                pt_idx.push(id);
            }
        }
        
        // add points to second face
        while(!pt_idx.empty()){
            unsigned int id = pt_idx.front(); pt_idx.pop();
            double dist = dist_to_line(point_cloud, edge2, point_cloud[id]);
            if( dist > 0.0 ){
                f2.second.push( chull_pair(dist, id) );
            }
        }
        
        // add faces to a queue and use to process the convex hull
        std::queue<face_set> faces;
        faces.push(f1);
        faces.push(f2);
        
        // main loop to come up with convex hull
        while( !faces.empty() ){
            
            // grab face off of queue
            face_set & face = faces.front();
            
            // check if the face's corresponding heap is empty. if so, add face to the chull list
            if( face.second.empty() ){ chull_edge_list.push_back(face.first); }
            
            // if the heap is not empty, find the furthest point in the point set, create two new
            // edges, and create two new heaps with the applicable points for those edges
            else{
                
                // get point with furthest distance from face
                auto pt_id = face.second.top().second; face.second.pop();
                
                // create two new faces
                f1.first = maze::point(face.first.first, pt_id);
                f1.second = max_heap();
                f2.first = maze::point(pt_id, face.first.second);
                f2.second = max_heap();
                
                // add points to first face
                while( !face.second.empty() ){
                    unsigned int id = face.second.top().second; face.second.pop();
                    double dist = dist_to_line(point_cloud, f1.first, point_cloud[id]);
                    if( dist > 0.0 ){
                        f1.second.push( chull_pair(dist, id) );
                    }else{
                        pt_idx.push(id);
                    }
                }
                
                // add points to second face
                while(!pt_idx.empty()){
                    unsigned int id = pt_idx.front(); pt_idx.pop();
                    double dist = dist_to_line(point_cloud, f2.first, point_cloud[id]);
                    if( dist > 0.0 ){
                        f2.second.push( chull_pair(dist, id) );
                    }
                }
                
                // add faces to the queue
                faces.push(f1);
                faces.push(f2);
                
            }// end if
            
            // pop face off the top of the queue
            faces.pop();
            
        }// end while
    }// end method
    
    
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
                get_chull( unvisited_pts, chull_edges );
                
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
            for(unsigned int i = 0; i < unvisited_pts.size(); ++i){
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
            }// end for
            
            if( !setDistance ){ min_interior_dist = 1.0; }
            
            // compute overall distance estimate based on convex hull and penalization of unvisited goals
            cost = ( chull_dist + (scale_*min_interior_dist)*static_cast<double>(num_interior_goals) );
            
        }else{ custom::exception("A* Convex Hull Distance Heuristic does not have a reference maze, cannot compute distance properly."); }
        return cost;
    }
    
}

/*
 *  convex_hull.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 10/1/17
 *  Purpose:
 *
 */

#include "convex_hull.hpp"
#include <cmath>
#include <queue>

typedef std::pair<double,unsigned int> chull_pair;

inline int abs( int num ){
    return num < 0 ? -num : num ;
}

double chull::dist_between_pts( const maze::point & p1, const maze::point & p2, bool useL1 ) {
    // compute the euclidean distance between points
    double dx = (double)p2.first - (double)p1.first;
    double dy = (double)p2.second - (double)p1.second;
    
    if( useL1 ){ return abs(dx) + abs(dy); }
    return sqrt(dx*dx + dy*dy);
}

double chull::dist_to_line( const std::vector<maze::point> & point_cloud, const maze::point & edge, const maze::point & pt ) {
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

void chull::get_chull( const std::vector<maze::point> & point_cloud, std::vector<maze::point> & chull_edge_list ) {
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

/*
 *  bfs_planner.hpp
 *
 *  Project: MP1
 *  Author : Liuyi Shi
 *  Date   : 9/16/17
 *  Purpose:
 *
 */

#ifndef bfs_planner_hpp
#define bfs_planner_hpp

#include "path_planner_interface.hpp"
#include <map>


namespace bfs {
    
template<typename transition_model>
class planner : public path_planner {
public:
    
    typedef typename transition_model::state_t state_t;
    
    //ctor/dtor
    planner();
    ~planner() = default;
    
    // method to return the path planner name for potential use in
    // post processing or automated testing work
    std::string plannerName() const;
    
    void computePath( const maze & maze_, path & path_ ) const;
    
};
    
}
#include "bfs_planner.hxx"
#endif /* bfs_planner_hpp */

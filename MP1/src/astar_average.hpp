/*
 *  astar_average.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/19/17
 *  Purpose:
 *
 */

#ifndef astar_average_hpp
#define astar_average_hpp

#include "astar_heuristic.hpp"
#include "multi_state.hpp"

namespace astar {
    
    // heuristic function for A* based on the manhatten distance
    class average : public heuristic_func_base<multi::state> {
    public:
        
        // ctor/dtor
        average();
        ~average() = default;
        
        // get name of heuristic
        std::string name() const;
        
        // distance measure
        double operator()( const multi::state & s ) const;
        
        void setScaleFactor( double scale );
        
    private:
        
        double scale_;
        
    };
    
}

#endif /* astar_average_hpp */

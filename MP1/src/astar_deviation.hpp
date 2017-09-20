/*
 *  astar_deviation.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/19/17
 *  Purpose:
 *
 */

#ifndef astar_deviation_hpp
#define astar_deviation_hpp

#include "astar_heuristic.hpp"

namespace astar {
    
    // heuristic function for A* based on the manhatten distance
    class deviation : public heuristic_func_base {
    public:
        
        // ctor/dtor
        deviation();
        ~deviation() = default;
        
        // get name of heuristic
        std::string name() const;
        
        // distance measure
        unsigned int operator()( const multi::state & s ) const;
        
        void setScaleFactor( double scale );
        
    private:
        
        double scale_;
        
    };
    
}

#endif /* astar_deviation_hpp */

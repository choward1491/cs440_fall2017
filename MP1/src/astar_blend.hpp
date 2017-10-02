/*
 *  astar_blend.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 10/1/17
 *  Purpose:
 *
 */

#ifndef astar_blend_hpp
#define astar_blend_hpp

#include "astar_heuristic.hpp"
#include "multi_state.hpp"

namespace astar {
    
    // heuristic function for A* based on the manhatten distance
    class blend : public heuristic_func_base<multi::state> {
    public:
        
        // ctor/dtor
        blend();
        ~blend() = default;
        
        // get name of heuristic
        std::string name() const;
        
        // distance measure
        double operator()( const multi::state & s ) const;
        
    private:
        
    };
    
}

#endif /* astar_blend_hpp */

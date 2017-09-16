/*
 *  transition_model.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/16/17
 *  Purpose:
 *
 */

#ifndef transition_model_hpp
#define transition_model_hpp

#include "maze.hpp"

namespace transition {
    
    // state transition model
    class model {
    public:
        
        // ctor/dtor
        model( const maze * mref = nullptr );
        ~model() = default;
        
        // define state transition operator
        maze::id_type operator()( maze::id_type current_state, maze::Actions current_action );
        
    private:
        const maze* maze_ref;
    };
    
}

#endif /* transition_model_hpp */

/*
 *  pong_bundle.hpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#ifndef pong_bundle_hpp
#define pong_bundle_hpp

#include "logic/pong_game.hpp"
#include "gui/pong_gui.hpp"

namespace pong {
    
    void initAllegro();
    
    class bundle {
    public:
        
        bundle(int window_width, int window_height);
        ~bundle() = default;
        
    private:
        game    game_;
        gui     gui_;
    };
}

#endif /* pong_bundle_hpp */

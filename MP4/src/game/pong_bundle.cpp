/*
 *  pong_bundle.cpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#include "pong_bundle.hpp"
#include "../utility/custom_exception.hpp"

namespace pong {
    
    /*
     game    game_;
     gui     gui_;
     */
    void initAllegro() {
        if(!al_init()) {
            throw custom::exception("Cannot initialize Allegro!");
        }
    }
    
    bundle::bundle(int window_w, int window_h):game_(&gui_),gui_(window_w,window_h){
        for(int i = 0; i < 10; ++i){
            game_.play(Predefined, Human);
        }
    }

}

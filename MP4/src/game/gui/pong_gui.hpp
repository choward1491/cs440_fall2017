/*
 *  pong_gui.hpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#ifndef pong_gui_hpp
#define pong_gui_hpp

// include standard libs
#include <vector>

// include external libs
#include <allegro5/allegro.h>


namespace pong {
    
    class gui {
    public:
        // useful typedefs
        typedef ALLEGRO_DISPLAY display_t;
        
        // ctor/dtor
        gui(int width = 700, int height = 600);
        ~gui();
        
        // draw pong game
        void drawGame( const std::vector<double> & state,
                      unsigned int playerPoints[2],
                      bool playersAreWalls[2] );
        
        // draw the menu
        void drawMenu();
        
        // getters
        display_t* getDisplayRef() const;
        
        
    private:
        display_t *display;
        double window_width, window_height;
        float paddle_width;
    };
    
}

#endif /* pong_gui_hpp */

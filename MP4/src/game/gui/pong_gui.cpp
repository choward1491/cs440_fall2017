/*
 *  pong_gui.cpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <cmath>

#include "pong_gui.hpp"
#include "../../utility/custom_exception.hpp"
#include "../logic/state_hasher.hpp"

namespace pong {
    
    //display_t *display;
    
    // useful typedefs
    //typedef ALLEGRO_DISPLAY display_t;
    
    // ctor/dtor
    gui::gui(int w, int h):display(nullptr),window_width(2.0*w),window_height(2.0*h),paddle_width(10.0) {
        display = al_create_display(window_width*0.5, window_height*0.5);
        if(!display) {
            throw custom::exception("Failed to make the display!");
        }
        
        if( !al_init_font_addon() ) { throw custom::exception("Failed to setup font addon!"); }
        if( !al_init_ttf_addon() )  { throw custom::exception("Failed to setup TTF addon!"); }
    }
    gui::~gui() {
        if(display){
            al_destroy_display(display);
        }
    }
    
    // draw pong game
    void gui::drawGame( const std::vector<double> & state,
                  unsigned int playerPoints[2],
                  bool playersAreWalls[2] )
    {
        // player1 and player2
        const int Player1 = 0, Player2 = 1;
        
        // set the backbuffer as the display
        al_set_target_backbuffer(display);
        
        // draw background
        al_clear_to_color(al_map_rgb_f(0.0, 0.0, 0.0));
        
        // draw left paddle/wall
        if( playersAreWalls[Player2] ){
            al_draw_filled_rectangle(0.0, 0.0, paddle_width, window_height,
                              al_map_rgb_f(1.0, 1.0, 1.0));
        }else{
            // get the y bounds for the paddle
            const double paddle_ydim[2] = { window_height*(state[RL::OpponentPaddle_y]),
                                            window_height*(state[RL::OpponentPaddle_y]+paddle_height)};
            
            // draw the paddle
            al_draw_filled_rectangle(0.0, paddle_ydim[0],
                              paddle_width, paddle_ydim[1],
                              al_map_rgb_f(1.0, 1.0, 1.0));
        }
        
        // draw right paddle/wall
        // draw left paddle/wall
        if( playersAreWalls[Player1] ){
            al_draw_filled_rectangle(window_width-paddle_width, 0.0, window_width, window_height,
                              al_map_rgb_f(1.0, 1.0, 1.0));
        }else{
            // get the y bounds for the paddle
            const double paddle_ydim[2] = { window_height*(state[RL::FriendlyPaddle_y]),
                                            window_height*(state[RL::FriendlyPaddle_y]+paddle_height)};
            
            // draw the paddle
            al_draw_filled_rectangle(window_width-paddle_width, paddle_ydim[0],
                              window_width, paddle_ydim[1],
                              al_map_rgb_f(1.0, 1.0, 1.0));
        }
        
        // draw the score
        char score[16] = {'\0'};
        ALLEGRO_FONT* font = al_load_ttf_font("/Library/Fonts/Tahoma.ttf", 40, 0);
        sprintf(score, "%u",playerPoints[Player2]);
        al_draw_text(font,
                     al_map_rgb_f(0.6, 0.0, 1.0),
                     window_width*0.25, window_height*0.1,
                     ALLEGRO_ALIGN_CENTRE, score);
        
        sprintf(score, "%u",playerPoints[Player1]);
        al_draw_text(font,
                     al_map_rgb_f(0.6, 0.0, 1.0),
                     window_width*0.75, window_height*0.1,
                     ALLEGRO_ALIGN_CENTRE, score);
        
        al_destroy_font(font);
        
        // draw middle barrier
        al_draw_line(window_width*0.5, 0.0,
                     window_width*0.5, window_height,
                     al_map_rgb_f(0.2, 0.2, 0.2), 1.0);
        
        // draw ball
        al_draw_filled_circle(state[RL::Ball_x]*window_width, state[RL::Ball_y]*window_height, 10.0,
                       al_map_rgb_f(0.0, 1.0, 1.0));
        
        // flip the display
        al_flip_display();
    }
    
    // draw the menu
    void gui::drawMenu() {
        
    }
    
    
    // getters
    typename gui::display_t* gui::getDisplayRef() const {
        return display;
    }
    
}

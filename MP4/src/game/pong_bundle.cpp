/*
 *  pong_bundle.cpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#include <algorithm>
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
    
    agent_type getTypeFromString( const std::string & type_s ){
        if( type_s == "human" )         {   return Human; }
        else if( type_s == "qagent")    {   return QAgent; }
        else if( type_s == "predefined"){   return Predefined; }
        else{                               return Wall; }
    }
    
    bundle::bundle(int window_w, int window_h):game_(&gui_),gui_(window_w,window_h),pconfig(nullptr){
        
    }
    
    void bundle::run(){
        // get types for players from config file
        std::string type1 = pconfig->retrieve<std::string>("player1");
        std::string type2 = pconfig->retrieve<std::string>("player2");
        std::transform(type1.begin(), type1.end(), type1.begin(), ::tolower);
        std::transform(type2.begin(), type2.end(), type2.begin(), ::tolower);
        agent_type p1_t = getTypeFromString(type1), p2_t = getTypeFromString(type2);
        
        // reset the game
        game_.reset();
        
        // play the game until someone ends it
        while( !game_.play(p1_t, p2_t) ){}
    }
    
    const game & bundle::getGame() const {
        return game_;
    }
    const gui & bundle::getGUI() const {
        return gui_;
    }
    
    void bundle::setConfig( const parser::config & pconfig_ ) {
        pconfig = &pconfig_;
        game_.setConfigFileContents(&pconfig_);
    }

}

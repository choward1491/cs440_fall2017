/*
 *  gif_wrapper.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/26/17
 *  Purpose:
 *
 */

#ifndef gif_wrapper_hpp
#define gif_wrapper_hpp

#include <utility>
#include <string>
#include "image.hpp"

namespace wrap {
    
    enum tile_t { Background = 0, Wall, Goal, Agent, Box};
    
    class gif {
    public:
        gif( uint32_t width = 1, uint32_t height = 1);
        ~gif();
        
        // getters/setters
        void setDims( uint32_t width, uint32_t height );
        std::pair<uint32_t,uint32_t> getDims() const;
        
        void setFrameRate( uint32_t rateHz );
        void addFrame( const img::image & img_ );
        void init( const std::string & filename );
        void save();
        
        
    private:
        uint32_t w, h;
        uint32_t delay;
        
        
    };
}

#endif /* gif_wrapper_hpp */

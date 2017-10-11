/*
 *  gif_wrapper.cpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/26/17
 *  Purpose:
 *
 */

#include "gif_wrapper.hpp"
#include "gif.h"

/*
 uint32_t r, c;
 uint32_t frameRateHz;
 GifWriter gwriter;*/

namespace wrap {

    GifWriter gwriter;
    
    gif::gif( uint32_t width, uint32_t height):w(width),h(height),delay(10) {
        gwriter.f           = nullptr;
        gwriter.oldImage    = nullptr;
        gwriter.firstFrame  = false;
    }
    gif::~gif() {
        if( gwriter.f ){ GifEnd(&gwriter); }
    }

    // getters/setters
    void gif::setDims( uint32_t width, uint32_t height ) {
        w = width;
        h = height;
    }
    std::pair<uint32_t,uint32_t> gif::getDims() const {
        return std::pair<uint32_t, uint32_t>(w,h);
    }

    void gif::setFrameRate( uint32_t rateHz ) {
        delay = 100 / rateHz;
    }
    void gif::addFrame( const img::image & img_ ) {
        auto iw = img_.width();
        auto ih = img_.height();
        auto rs = 4*iw;
        std::vector<uint8_t> image_bytes(4*iw*ih);
        for(uint32_t i = 0; i < image_bytes.size(); i+=4){
            unsigned int r = (i/4)%iw, c = i/rs;
            auto px = img_.pixelAt(r, c);
            image_bytes[i + 0] = (uint8_t)UINT8_MAX*px.r();
            image_bytes[i + 1] = (uint8_t)UINT8_MAX*px.g();
            image_bytes[i + 2] = (uint8_t)UINT8_MAX*px.b();
            image_bytes[i + 3] = (uint8_t)UINT8_MAX*px.a();
        }
        
        GifWriteFrame(&gwriter, &image_bytes[0], w, h, delay);
    }
    void gif::init( const std::string & filename ) {
        GifBegin(&gwriter, filename.c_str(), w, h, delay);
    }
    void gif::save() {
        GifEnd(&gwriter);
    }

}

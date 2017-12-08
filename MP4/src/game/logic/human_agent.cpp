/*
 *  human_agent.cpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#include "human_agent.hpp"
#include "../../RL/pong_mdp.hpp"
#include "state_hasher.hpp"

namespace pong {
    double human::updatePosition( const std::vector<double> & state ) {
        
        // define output position value
        double pos = state[RL::FriendlyPaddle_y];
        
        // define useful values
        const double delta_step[3]  = {0.0, 0.04, -0.04};
        
        // update the position of the paddle
        pos += delta_step[kpressed]; kpressed = None;
        if( pos < 0 )          { pos = 0; }
        else if( pos > height ){ pos = height; }
        return pos;
    }
    enum agent_type human::getAgentType() const { return Human; }
    void human::setKeyInput( enum keyInput k ) {
        kpressed = k;
    }
}// end namespace pong

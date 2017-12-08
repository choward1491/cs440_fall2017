/*
 *  predefined_agent.cpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#include "predefined_agent.hpp"
#include "../../RL/pong_mdp.hpp"

namespace pong {
    double predefined::updatePosition( const std::vector<double> & state ) {
        
        // define output position value
        double pos = 0.0;
        
        // define useful values
        const double delta_step[3]  = {0.0, 0.04, -0.04};
        const double height         = 1.0;
        
        // difference between vertical ball pos and opponent paddle vertical pos
        double dy = state[RL::Ball_y] - state[RL::FriendlyPaddle_y];
        
        // update paddle position
        if( dy > 0 ){
            pos += 0.5*delta_step[RL::PaddleUp];
        }else if ( dy < 0 ){
            pos += 0.5*delta_step[RL::PaddleDown];
        }
        
        // bound the opponent's paddle pos
        if( pos < 0 )          { pos = 0; }
        else if( pos > height ){ pos = height; }
        return pos;
    }
    enum agent_type predefined::getAgentType() const { return Predefined; }
}// end namespace pong

/*
 *  q_agent.cpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#include "q_agent.hpp"
#include "state_hasher.hpp"
#include "../../utility/binary_search.hpp"

namespace pong {
    
    q_agent::q_agent(){
        
    }
    
    double q_agent::updatePosition( const std::vector<double> & state ) {
        
        // define output position value
        double pos = state[RL::FriendlyPaddle_y];
        
        // define useful values
        const double delta_step[3]  = {0.0, 0.04, -0.04};
        
        // get state hash
        auto shash = getStateHash(state,controller.getMDP().getVariableValues());
        
        // update the position of the paddle
        pos += delta_step[controller.argmaxQ(shash)];
        if( pos < 0 )                           { pos = 0; }
        else if( pos > height - paddle_height)  { pos = (height - paddle_height); }
        return pos;
    }
    enum agent_type q_agent::getAgentType() const { return QAgent; }
    
    void q_agent::setMDP( const RL::mdp<> & mdp ) {
        controller.getMDP() = mdp;
        controller.init();
    }
    void q_agent::loadController( const std::string & filename ) {
        controller.load(filename);
    }
    typename q_agent::controller_t & q_agent::getController() {
        return controller;
    }
    const typename q_agent::controller_t & q_agent::getController() const {
        return controller;
    }
}// end namespace pong

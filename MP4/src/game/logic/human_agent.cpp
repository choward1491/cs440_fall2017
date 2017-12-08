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

namespace pong {
    double human::updatePosition( const std::vector<double> & state ) {
        return state[RL::FriendlyPaddle_y];
    }
    enum agent_type human::getAgentType() const { return Human; }
}// end namespace pong

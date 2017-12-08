/*
 *  wall_agent.cpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#include "wall_agent.hpp"

namespace pong {
    double wall::updatePosition( const std::vector<double> & state ) {
        return 0.0;
    }
    enum agent_type wall::getAgentType() const { return Wall; }
}// end namespace pong

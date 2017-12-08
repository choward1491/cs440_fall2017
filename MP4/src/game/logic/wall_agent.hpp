/*
 *  wall_agent.hpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#ifndef wall_agent_hpp
#define wall_agent_hpp

#include "pong_agent.hpp"

namespace pong {
    class wall : public agent {
    public:
        double updatePosition( const std::vector<double> & state );
        enum agent_type getAgentType() const;
    };
}// end namespace pong

#endif /* wall_agent_hpp */

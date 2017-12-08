/*
 *  predefined_agent.hpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#ifndef predefined_agent_hpp
#define predefined_agent_hpp

#include "pong_agent.hpp"

namespace pong {
    class predefined : public agent {
    public:
        ~predefined() = default;
        double updatePosition( const std::vector<double> & state );
        enum agent_type getAgentType() const;
    };
}// end namespace pong

#endif /* predefined_agent_hpp */

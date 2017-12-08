/*
 *  human_agent.hpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#ifndef human_agent_hpp
#define human_agent_hpp

#include "pong_agent.hpp"

namespace pong {
    class human : public agent {
    public:
        enum keyInput: int { None = 0, Up, Down };
        ~human() = default;
        double updatePosition( const std::vector<double> & state );
        void setKeyInput( enum keyInput k );
        enum agent_type getAgentType() const;
    private:
        enum keyInput kpressed;
    };
}// end namespace pong

#endif /* human_agent_hpp */

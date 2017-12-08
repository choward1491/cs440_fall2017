/*
 *  pong_agent.hpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#ifndef pong_agent_hpp
#define pong_agent_hpp

#include <vector>

namespace pong {
    
    enum agent_type: unsigned int {
        Wall = 0,
        Predefined,
        QAgent,
        Human
    };
    
    class agent {
    public:
        
        // ctor/dtor
        agent() = default;
        virtual ~agent() = default;
        
        // update position
        virtual double updatePosition( const std::vector<double> & state )  = 0;
        virtual enum agent_type getAgentType() const                        = 0;
        
    };
}

#endif /* pong_agent_hpp */

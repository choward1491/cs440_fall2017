/*
 *  q_agent.hpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#ifndef q_agent_hpp
#define q_agent_hpp

#include "pong_agent.hpp"
#include "../../RL/q_learning.hpp"
#include "../../RL/pong_mdp.hpp"

namespace pong {
    class q_agent : public agent {
    public:
        // useful typedef
        typedef RL::mdp<>                   mdp_t;
        typedef ::RL::q_learning<RL::mdp<>> controller_t;
        
        // ctor
        q_agent();
        ~q_agent() = default;
        
        // function to update position
        double updatePosition( const std::vector<double> & state );
        enum agent_type getAgentType() const;
        
        // helper q_agent functionality
        void setMDP( const RL::mdp<> & mdp );
        void loadController( const std::string & filename );
        controller_t & getController();
        const controller_t & getController() const;
        
    private:
        controller_t controller;
    };
}// end namespace pong

#endif /* q_agent_hpp */

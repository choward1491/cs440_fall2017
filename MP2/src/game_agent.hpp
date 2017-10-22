/*
 *  game_agent.hpp
 *
 *  Project: MP2
 *  Author : Christian J Howard
 *  Date   : 10/21/17
 *  Purpose:
 *
 */

#ifndef SRC_GAME_AGENT_HPP
#define SRC_GAME_AGENT_HPP

#include <limits>

namespace game {

    // class to represent a utility evaluator for states
    template<typename Rules, typename num_val = float>
    class evaluator {
    public:

        // useful typedefs
        typedef Rules::state_t          state_t;
        typedef Rules::action_t         action_t;
        typedef Rules::transition_t     transition_t;
        typedef Rules                   rules_t;
        typedef num_val                 eval_t;

        // compute estimated state utility
        virtual ~evaluator()                                = default;
        virtual eval_t utilityEstimate( const state_t & s ) = 0;
    };


    // a class to represent an agent striving to play a game
    // based on Rules
    template<typename Rules, typename num_val = float>
    class agent {
    public:

        // useful typedefs
        typedef Rules::state_t              state_t;
        typedef Rules::action_t             action_t;
        typedef Rules::transition_t         transition_t;
        typedef Rules                       rules_t;
        typedef num_val                     eval_t;
        typedef evaluator<rules_t,num_val>  evaluator_t;

        // define pure virtual method for agent class
        virtual ~agent()                                        = default;
        virtual action_t    getNextMove( const state_t & s )    = 0;

        // define other helpful methods
        void setUtilityEstimator( evaluator_t & e ){ evaluator_ = &e; }
        void setTeam( int team_value ) { team_assignment = team_value; }

    protected:
        int          getTeam() { return team_assignment; }
        evaluator_t* getEvaluator() { return evaluator_; }

    private:
        int team_assignment;
        evaluator_t* evaluator_;

    };

}


#endif //SRC_GAME_AGENT_HPP

//
// Created by Christian J Howard on 10/21/17.
//

#ifndef SRC_BREAKTHROUGH_MINIMAX_AGENT_HPP
#define SRC_BREAKTHROUGH_MINIMAX_AGENT_HPP

#include "game_agent.hpp"
#include "breakthrough_rules.hpp"

namespace bt {

    template<typename Rules>
    class minimax : public game::agent<Rules> {
    public:
        typedef game::agent<Rules> parent;
        typedef parent::state_t     state_t;
        typedef parent::action_t    action_t;
        typedef parent::eval_t      utility_t;
        virtual action_t    getNextMove( const state_t & s ) {
            return minimaxDecision(s);
        }

    private:
        action_t minimaxDecision( const state_t & s ){
            action_t    a;
            utility_t   u = std::numeric_limits<utility_t>::min();
            // get set of actions for state
            // sort actions based on something
            // loop through actions and compute best action
        }

        utility_t maxDecision( const state_t & s ){
            if( rules.isGameComplete(s) ){ return rules.evalFinalUtility(s); }
            utility_t value = std::numeric_limits<utility_t>::min();
            // get set of actions for state
            // sort actions based on something
            // for each action compute maximum min-value
            return value;
        }

        utility_t minDecision( const state_t & s ){
            if( rules.isGameComplete(s) ){ return rules.evalFinalUtility(s); }
            utility_t value = std::numeric_limits<utility_t>::max();
            // get set of actions for state
            // sort actions based on something
            // for each action compute minimum max-value
            return value;
        }

        Rules rules;

    };

}

#endif //SRC_BREAKTHROUGH_MINIMAX_AGENT_HPP

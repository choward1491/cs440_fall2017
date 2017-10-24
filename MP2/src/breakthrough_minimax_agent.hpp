//
// Created by Christian J Howard on 10/21/17.
//

#ifndef SRC_BREAKTHROUGH_MINIMAX_AGENT_HPP
#define SRC_BREAKTHROUGH_MINIMAX_AGENT_HPP

#include "game_agent.hpp"
#include "breakthrough_rules.hpp"
#include <vector>

namespace bt {

    template<typename Rules>
    class minimax : public game::agent<Rules> {
    public:
        typedef game::agent<Rules>            parent;
        typedef typename parent::state_t      state_t;
        typedef typename parent::action_t     action_t;
        typedef typename parent::eval_t       utility_t;
        typedef typename parent::actions_t    actions_t;
        typedef typename parent::transition_t transition_t;

        virtual action_t    getNextMove( const state_t & s ) {
            return minimaxDecision(s);
        }

    private:

        action_t minimaxDecision( const state_t & s ){
            action_t    a;
            utility_t   u = std::numeric_limits<utility_t>::lowest();

            // get set of actions for state
            actions_t action_set;
            rules.getValidActionSet( s, this->getTeam(), action_set );

            // loop through actions and compute best action
            utility_t umax  = utility_t(0);
            int rel_depth   = 1;
            for(auto action : action_set ){
                umax = minDecision(F(s, action), rel_depth);
                if( umax > u ){ a = static_cast<action_t>(action); u = umax; }
            }

            return a;
        }

        utility_t maxDecision( const state_t & s, int depth){
            if( rules.isGameComplete(s) )       { return rules.template evalFinalUtility<utility_t>(s, this->getTeam()); }
            if( depth == this->getMaxSearchDepth() )  { return this->getEvaluator()->utilityEstimate(s, this->getTeam()); }
            utility_t u = std::numeric_limits<utility_t>::lowest();

            // get set of actions for state
            actions_t action_set;
            rules.getValidActionSet( s, this->getTeam(), action_set );

            // for each action compute minimum max-value
            for(auto action : action_set ){
                u = std::max(u, minDecision(F(s, action), depth+1) );
            }

            return u;
        }

        utility_t minDecision( const state_t & s, int depth){
            if( rules.isGameComplete(s) )       { return rules.template evalFinalUtility<utility_t>(s, this->getTeam()); }
            if( depth == this->getMaxSearchDepth() )  { return this->getEvaluator()->utilityEstimate(s, this->getTeam()); }
            utility_t u = std::numeric_limits<utility_t>::max();

            // get set of actions for state
            actions_t action_set;
            rules.getValidActionSet( s, this->getOpposingTeam(), action_set );

            // for each action compute minimum max-value
            for(auto action : action_set ){
                u = std::min(u, maxDecision(F(s, action), depth+1) );
            }

            return u;
        }

        Rules rules;
        transition_t F;

    };

}

#endif //SRC_BREAKTHROUGH_MINIMAX_AGENT_HPP

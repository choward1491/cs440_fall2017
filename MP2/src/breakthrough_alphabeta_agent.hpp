//
// Created by Christian J Howard on 10/24/17.
//

#ifndef SRC_BREAKTHROUGH_ALPHABETA_AGENT_HPP
#define SRC_BREAKTHROUGH_ALPHABETA_AGENT_HPP

#include "game_agent.hpp"
#include "breakthrough_rules.hpp"
#include <vector>

namespace bt {

    template<typename Rules>
    class alphabeta : public game::agent<Rules> {
    public:
        typedef game::agent<Rules>            parent;
        typedef typename parent::state_t      state_t;
        typedef typename parent::action_t     action_t;
        typedef typename parent::eval_t       utility_t;
        typedef typename parent::actions_t    actions_t;
        typedef typename parent::transition_t transition_t;

        virtual int    getNextMove( const state_t & s ) {
            return minimaxDecision(s);
        }

    private:

        int minimaxDecision( const state_t & s ){
            action_t    a;
            utility_t   u = std::numeric_limits<utility_t>::lowest();

            // get set of actions for state
            actions_t action_set;
            rules.getValidActionSet( s, this->getTeam(), action_set );

            // sort actions in some manner
            std::vector<action_t> priority_actions;

            // loop through actions and compute best action
            utility_t umax  = utility_t(0);
            int rel_depth   = 1;
            for(auto action : priority_actions ){
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

            // sort actions in some manner
            std::vector<action_t> priority_actions;

            // for each action compute minimum max-value
            for(auto action : priority_actions ){
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

            // sort actions in some manner
            std::vector<action_t> priority_actions;

            // for each action compute minimum max-value
            for(auto action : priority_actions ){
                u = std::min(u, maxDecision(F(s, action), depth+1) );
            }

            return u;
        }

        Rules rules;
        transition_t F;

    };

}

#endif //SRC_BREAKTHROUGH_ALPHABETA_AGENT_HPP

//
// Created by Christian J Howard on 10/24/17.
//

#ifndef SRC_BREAKTHROUGH_UTILITY_AB_HPP
#define SRC_BREAKTHROUGH_UTILITY_AB_HPP

#include "breakthrough_alphabeta_agent.hpp"
#include <algorithm>

namespace bt {

    template<typename Rules>
    class utility_ab : public alphabeta<Rules> {
    private:

        typedef alphabeta<Rules> parent;
        typedef typename parent::state_t      state_t;
        typedef typename parent::action_t     action_t;
        typedef typename parent::eval_t       utility_t;
        typedef typename parent::actions_t    actions_t;
        typedef typename parent::transition_t transition_t;
        typedef typename parent::evaluator_t  evaluator_t;

        static bool sortAgentTeam(int i, int j){
            return (*uref)[i] > (*uref)[j];
        }

        static bool sortOpponentTeam(int i, int j){
            return (*uref)[i] < (*uref)[j];
        }

        static std::vector<utility_t>* uref;

        void prioritizeActions( const state_t & s, const actions_t & actions, std::vector<int> & pactions, bool isAgentsTeam ) {

            evaluator_t* eval = this->getEvaluator();
            std::vector<int> inds(actions.size()), atmp(actions.size());
            pactions.clear();
            std::vector<utility_t> util_estimates(actions.size());
            uref = &util_estimates;
            int i = 0, team = this->getTeam();
            for(auto action : actions ) {
                atmp[i] = action;
                inds[i] = i;
                util_estimates[i++] = eval->utilityEstimate(this->F(s,action),team);
            }

            // sort the actions based on the utility values
            if( isAgentsTeam ){
                std::sort(inds.begin(),inds.end(),sortAgentTeam);
            }else{
                std::sort(inds.begin(),inds.end(),sortOpponentTeam);
            }

            // create new actions based on the sorting
            for(auto idx: inds){ pactions.push_back(atmp[idx]); }
        }

    };

    template<typename Rules>
    std::vector<typename utility_ab<Rules>::utility_t>* utility_ab<Rules>::uref = nullptr;

}

#endif //SRC_BREAKTHROUGH_UTILITY_AB_HPP

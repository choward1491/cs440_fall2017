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
        typedef typename parent::evaluator_t  evaluator_t;
        typedef std::pair<utility_t,int>      ua_t;

        virtual std::string name() const { return "Alpha-Beta"; }

        virtual int    getNextMove( const state_t & s ) {

            // update the index for this move
            this->moveNum++;

            // get starting time for move
            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

            // do move
            uint32_t numExpandedNodes = 0;
            int move = alphabetaDecision(s, numExpandedNodes);

            // get end time for move
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            auto time_span = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

            // do moving average estimate for avg move time
            this->avgMoveTimeMilliseconds = (time_span.count() + this->avgMoveTimeMilliseconds*(this->moveNum-1))/static_cast<double>(this->moveNum);

            // do running average for avg number of expansions per move
            this->avgExpandedNodes = (numExpandedNodes + this->avgExpandedNodes*(this->moveNum-1))/static_cast<double>(this->moveNum);

            // add nodes expanded on this move to total nodes expanded
            this->numNodesExpanded += numExpandedNodes;

            // return the move chosen
            return move;
        }

    protected:
        Rules rules;
        transition_t F;

    private:

        virtual void prioritizeActions( const state_t & s, const actions_t & actions, std::vector<int> & pactions, bool isAgentsTeam ) {
            pactions.clear();
            for(auto action : actions ) {
                pactions.push_back(action);
            }
        }


        int alphabetaDecision( const state_t & s , uint32_t & numExpandedNodes ){
            utility_t alpha = std::numeric_limits<utility_t>::lowest();
            utility_t beta  = std::numeric_limits<utility_t>::max();
            ua_t bestResult = maxValue(s,alpha,beta,0, numExpandedNodes);
            return bestResult.second;
        }

        ua_t maxValue( const state_t & s, utility_t alpha, utility_t beta, int depth, uint32_t & numExpandedNodes ){

            // compute that we have expanded this node, be it through the evaluation
            // function or normal alpha-beta algorithms
            numExpandedNodes++;

            // check if the state has hit a final game state
            if( rules.isGameComplete(s) )       {
                return ua_t(rules.template evalFinalUtility<utility_t>(s, this->getTeam()),0);
            }

            // if this is the max depth, compute evaluation function
            if( depth == this->getMaxSearchDepth() )  {
                return ua_t(this->getEvaluator()->utilityEstimate(s, this->getTeam()),0);
            }
            ua_t tmp, best = ua_t(std::numeric_limits<utility_t>::lowest(),0);

            // get set of actions for state
            actions_t action_set;
            rules.getValidActionSet( s, this->getTeam(), action_set );

            // sort actions in some manner
            std::vector<int> priority_actions;
            prioritizeActions(s, action_set,priority_actions, true);

            // for each action compute minimum max-value
            for(auto action : priority_actions ){
                tmp = minValue(F(s, action), alpha, beta, depth+1, numExpandedNodes); tmp.second = action;
                if( tmp.first >= best.first ){ best = tmp; }
                if( best.first >= beta ){
                    return best; }
                alpha = std::max(alpha,best.first);
            }

            return best;
        }

        ua_t minValue( const state_t & s, utility_t alpha, utility_t beta, int depth, uint32_t & numExpandedNodes){

            // compute that we have expanded this node, be it through the evaluation
            // function or normal alpha-beta algorithms
            numExpandedNodes++;

            // check if the state has hit a final game state
            if( rules.isGameComplete(s) )       {
                return ua_t(rules.template evalFinalUtility<utility_t>(s, this->getTeam()),0);
            }

            // if this is the max depth, compute evaluation function
            if( depth == this->getMaxSearchDepth() )  {
                return ua_t(this->getEvaluator()->utilityEstimate(s, this->getTeam()),0);
            }
            ua_t tmp, best = ua_t(std::numeric_limits<utility_t>::max(),0);

            // get set of actions for state
            actions_t action_set;
            rules.getValidActionSet( s, this->getOpposingTeam(), action_set );

            // sort actions in some manner
            std::vector<int> priority_actions;
            prioritizeActions(s, action_set, priority_actions, false);

            // for each action compute minimum max-value
            for(auto action : priority_actions ){
                tmp = maxValue(F(s, action), alpha, beta, depth+1, numExpandedNodes); tmp.second = action;
                if( tmp.first <= best.first ){ best = tmp; }
                if( best.first <= alpha ){
                    return best; }
                beta = std::min(beta,best.first);
            }

            return best;
        }



    };

}

#endif //SRC_BREAKTHROUGH_ALPHABETA_AGENT_HPP

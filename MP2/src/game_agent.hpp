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
#include <chrono>
#include <string>

namespace game {

    // class to represent a utility evaluator for states
    template<typename Rules, typename num_val = double>
    class evaluator {
    public:

        // useful typedefs
        typedef typename Rules::state_t          state_t;
        typedef typename Rules::action_t         action_t;
        typedef typename Rules::actions          actions_t;
        typedef typename Rules::transition_t     transition_t;
        typedef Rules                            rules_t;
        typedef num_val                          eval_t;

        // compute estimated state utility
        virtual ~evaluator()                                = default;
        virtual eval_t utilityEstimate( const state_t & s, int team ) = 0;
        virtual std::string name() const { return "base_evaluator"; }
    };


    // a class to represent an agent striving to play a game
    // based on Rules
    template<typename Rules, typename num_val = double>
    class agent {
    public:

        // useful typedefs
        typedef typename Rules::state_t              state_t;
        typedef typename Rules::action_t             action_t;
        typedef typename Rules::actions              actions_t;
        typedef typename Rules::transition_t         transition_t;
        typedef Rules                                rules_t;
        typedef num_val                              eval_t;
        typedef evaluator<rules_t,num_val>           evaluator_t;

        // define pure virtual method for agent class
        agent():depth(1),team_assignment(0),evaluator_(nullptr),
                numNodesExpanded(0),moveNum(0),avgExpandedNodes(0),avgMoveTimeMilliseconds(0){}
        virtual ~agent()                                        = default;
        virtual int    getNextMove( const state_t & s )    = 0;
        virtual std::string name() const { return "base_agent"; }

        // define other helpful methods
        void setUtilityEstimator( evaluator_t & e ){ evaluator_ = &e; }
        void setTeam( int team_value ) { team_assignment = team_value; }
        void setMaxSearchDepth( int depth_ ){ depth = depth_; }
        uint32_t getNumNodesExpanded() const { return numNodesExpanded; }
        double getAvgExpandedNodes() const { return avgExpandedNodes; }
        double getAvgMoveTimeInMilliseconds() const { return avgMoveTimeMilliseconds; }
        void initPlayer(){
            moveNum                 = 0;
            numNodesExpanded        = 0;
            avgExpandedNodes        = 0;
            avgMoveTimeMilliseconds = 0;
        }

    protected:
        int          getMaxSearchDepth() const { return depth; }
        int          getTeam() const { return team_assignment; }
        int          getOpposingTeam() const {
            int del = team_assignment - 1;
            return del < 0 ? -del : del ;
        }
        evaluator_t* getEvaluator() const { return evaluator_; }

        uint32_t numNodesExpanded, moveNum;
        double avgExpandedNodes, avgMoveTimeMilliseconds;



    private:
        int team_assignment;
        int depth;
        evaluator_t* evaluator_;


    };

}


#endif //SRC_GAME_AGENT_HPP

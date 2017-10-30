//
// Created by Christian J Howard on 10/23/17.
//

#ifndef SRC_BREAKTHROUGH_PROVIDED_HEURISTICS_HPP
#define SRC_BREAKTHROUGH_PROVIDED_HEURISTICS_HPP

#include "game_agent.hpp"
#include "breakthrough_rules.hpp"
#include <random>

namespace bt {
    namespace provided {

        // setup random number generator for evaluators
        std::default_random_engine generator;
        std::uniform_real_distribution<double> U(0,1);



        // defensive heuristic provided by the MP definition
        template<int NR = 8, int NC = 8, template<int,int> typename Rules = bt::baseline_rules>
        class defensive : public game::evaluator<Rules<NR,NC>>{
        public:
            typedef game::evaluator<Rules<NR,NC>> parent;
            typedef typename parent::state_t state_t;
            typedef typename parent::eval_t eval_t;


            defensive(){ generator.seed(17); }

            virtual eval_t utilityEstimate( const state_t & s, int team ){
                enum teams: int { team1 = 0, team2 };
                int teamCount[3] = {0}, numTeam2inTeam1Base = 0, numTeam1inTeam2Base = 0;
                int nr = s.numRows()-1;
                for(int r = 0; r < s.numRows(); ++r){
                    for(int c = 0; c < s.numCols(); ++c){
                        if( r == 0 && s.getStateAt(r,c) == team2)       { ++numTeam2inTeam1Base; }
                        else if( r == nr && s.getStateAt(r,c) == team1) { ++numTeam1inTeam2Base; }
                        ++teamCount[s.getStateAt(r,c)];
                    }// end for c
                }// end for r

                return 2.0*teamCount[team] + U(generator);
            }
        };


        // offensive heuristic provided by the MP definition
        template<int NR = 8, int NC = 8, template<int,int> typename Rules = bt::baseline_rules>
        class offensive : public game::evaluator<Rules<NR,NC>>{
        public:

            typedef game::evaluator<Rules<NR,NC>> parent;
            typedef typename parent::state_t state_t;
            typedef typename parent::eval_t eval_t;

            offensive(){ generator.seed(17); }

            virtual eval_t utilityEstimate( const state_t & s, int team ){
                enum teams: int { team1 = 0, team2 };
                int teamCount[3] = {0}, numTeam2inTeam1Base = 0, numTeam1inTeam2Base = 0;
                int nr = s.numRows()-1;
                for(int r = 0; r < s.numRows(); ++r){
                    for(int c = 0; c < s.numCols(); ++c){
                        if( r == 0 && s.getStateAt(r,c) == team2)       { ++numTeam2inTeam1Base; }
                        else if( r == nr && s.getStateAt(r,c) == team1) { ++numTeam1inTeam2Base; }
                        ++teamCount[s.getStateAt(r,c)];
                    }// end for c
                }// end for r

                int opponent = team - 1 < 0 ? 1 : 0;
                return 2.0*(30 - teamCount[opponent]) + U(generator);
            }
        };

    }
}

#endif //SRC_BREAKTHROUGH_PROVIDED_HEURISTICS_HPP

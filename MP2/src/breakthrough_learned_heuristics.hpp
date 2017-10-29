//
// Created by Christian J Howard on 10/28/17.
//

#ifndef SRC_BREAKTHROUGH_LEARNED_HEURISTICS_HPP_HPP
#define SRC_BREAKTHROUGH_LEARNED_HEURISTICS_HPP_HPP

#include "game_agent.hpp"
#include "breakthrough_rules.hpp"
#include <cmath>
#include <algorithm>

namespace bt {
    namespace learned {

        enum feature_t {
            Pieces1 = 0,
            MeanDist1,
            StdDevDist1,
            AvgBarrier1,
            Pieces2,
            MeanDist2,
            StdDevDist2,
            AvgBarrier2
        };

        template<int NR, int NC, typename state_t>
        void getFeatures( double* feature_vec, const state_t & s ){
            /* Features being extracted will be the following:
                - Number of team1 pieces on board
                - Number of team2 pieces on board
                - avg distance of team1 to end of board
                - standard deviation of team1 distance to end of board
                - avg distance of team2 to end of board
                - standard deviation of team2 distance to end of board
                - team1 average barrier size
                - team2 average barrier size
             */
            const int numFeatures = 8;

            // variables to store useful information for features
            int numPieces[2] = {0}, numBarriers[2] = {0};
            double meanDist[2] = {0}, stdDist[2] = {0};
            double avgBarrierSize[2] = {0};

            // loop through board and compute features
            const int team1 = 0, team2 = 1, none = 2;
            const double invnr = 1.0 / static_cast<double>(NR-1);
            const double invnc = 1.0 / static_cast<double>(NC-1);
            int sidx = 0, type = none;
            double tv = 0;

            // loop through state and compute statistics
            for(int r = 0; r < NR; ++r){

                // reset type flag
                type = none;

                for(int c = 0; c < NC; ++c){

                    // compute feature data for team 1
                    if( s.getStateAt(r,c) == team1 ){
                        numPieces[team1]++;
                        tv = r * invnr;
                        meanDist[team1] += tv;
                        stdDist[team1] += tv*tv;
                        if( type != team1 ){
                            if( type == team2 ){
                                numBarriers[team2]++;
                                avgBarrierSize[team2] += (c - sidx);
                            }
                            sidx = c; type = team1;
                        }
                    }

                    //  compute feature data for team 2
                    else if( s.getStateAt(r,c) == team2) {
                        numPieces[team2]++;
                        tv = (NR - r - 1) * invnr;
                        meanDist[team2] += tv;
                        stdDist[team2] += tv*tv;
                        if( type != team2 ){
                            if( type == team1 ){
                                numBarriers[team1]++;
                                avgBarrierSize[team1] += (c - sidx);
                            }
                            sidx = c; type = team2;
                        }
                    }else{ type = none; }
                }// end for c

                // do clean up for the jth row
                if( type == team2 ){
                    numBarriers[team2]++;
                    avgBarrierSize[team2] += (NC - sidx);
                }else if( type == team1 ){
                    numBarriers[team1]++;
                    avgBarrierSize[team1] += (NC - sidx);
                }
            }// end for r


            // finalize computation for mean and standard deviations of stuff
            meanDist[team1] /= static_cast<double>(numPieces[team1]);
            meanDist[team2] /= static_cast<double>(numPieces[team2]);
            stdDist[team1] = std::sqrt(stdDist[team1]/static_cast<double>(numPieces[team1])
                                       - meanDist[team1]*meanDist[team1]);
            stdDist[team2] = std::sqrt(stdDist[team2]/static_cast<double>(numPieces[team2])
                                       - meanDist[team2]*meanDist[team2]);
            avgBarrierSize[team1] /= static_cast<double>(numBarriers[team1]);
            avgBarrierSize[team2] /= static_cast<double>(numBarriers[team2]);

            // set the feature vector
            feature_vec[Pieces1]        = numPieces[team1];
            feature_vec[MeanDist1]      = meanDist[team1];
            feature_vec[StdDevDist1]    = stdDist[team1];
            feature_vec[AvgBarrier1]    = avgBarrierSize[team1];
            feature_vec[Pieces2]        = numPieces[team2];
            feature_vec[MeanDist2]      = meanDist[team2];
            feature_vec[StdDevDist2]    = stdDist[team2];
            feature_vec[AvgBarrier2]    = avgBarrierSize[team2];
        }

        // defensive heuristic provided by the MP definition
        template<int NR = 8, int NC = 8,
                template<int,int> typename Rules = bt::baseline_rules>
        class defensive : public game::evaluator<Rules<NR,NC> >{
        public:
            typedef game::evaluator<Rules<NR,NC>> parent;
            typedef typename parent::state_t state_t;
            typedef typename parent::eval_t eval_t;


            defensive(){
#ifdef USE_PRECOMPUTED_DCOEFS
                // place to specify coefficient values for training
                // via some optimization algorithm
#endif
            }

            virtual eval_t utilityEstimate( const state_t & s, int team ){
                double utility = 0, features[9] = {0.0}; features[0] = 1.0;
                getFeatures<NR,NC,state_t>(&features[1],s);
                for(int i = 0; i < 5; ++i){ utility += coefs[i]*features[i]; }
                return utility;
            }

            void setCoefficient(int idx, double value ){ coefs[idx] = value; }
            int numCoefficients() const { return 5; }

        private:

            double coefs[5];

        };


        // offensive heuristic provided by the MP definition
        template<int NR = 8, int NC = 8,
                template<int,int> typename Rules = bt::baseline_rules>
        class offensive : public game::evaluator<Rules<NR,NC>> {
        public:

            typedef game::evaluator<Rules<NR,NC>> parent;
            typedef typename parent::state_t state_t;
            typedef typename parent::eval_t eval_t;

            offensive(){
#ifdef USE_PRECOMPUTED_OCOEFS
                // place to specify coefficient values for training
                // via some optimization algorithm
#endif
            }

            virtual eval_t utilityEstimate( const state_t & s, int team ){
                double utility = coefs[0], features[8] = {0.0};
                getFeatures<NR,NC,state_t>(features,s);
                for(int i = 0; i < 4; ++i){ utility += coefs[1+i]*features[4+i]; }
                return utility;
            }

            void setCoefficient(int idx, double value ){ coefs[idx] = value; }
            int numCoefficients() const { return 5; }

        private:

            double coefs[5];
        };
    }
}

#endif //SRC_BREAKTHROUGH_LEARNED_HEURISTICS_HPP_HPP

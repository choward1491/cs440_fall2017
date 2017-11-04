//
// Created by Christian J Howard on 10/30/17.
//

#ifndef SRC_BREAKTHROUGH_HANDCRAFT_HEURISTICS_HPP
#define SRC_BREAKTHROUGH_HANDCRAFT_HEURISTICS_HPP

#include "game_agent.hpp"
#include "breakthrough_rules.hpp"
#include "breakthrough_extended_rules.hpp"
#include <random>
#include <vector>
#include <cmath>
#include <algorithm>

namespace bt {
    namespace handcraft {

        // setup random number generator for evaluators
        std::default_random_engine generator;
        std::uniform_real_distribution<double> U(0,1);

        enum feature_t {
            Pieces1 = 0,
            MeanDist1,
            StdDevDist1,
            StdDevWidth1,
            AvgBarrier1,
            Pieces1InGoal,
            Pieces1InHome,
            Pieces1Almost,
            Pieces1Max,
            Pieces2,
            MeanDist2,
            StdDevDist2,
            StdDevWidth2,
            AvgBarrier2,
            Pieces2InGoal,
            Pieces2InHome,
            Pieces2Almost,
            Pieces2Max
        };

        enum feature_tl {
            Pieces = 0,
            MeanDist,
            StdDevDist,
            StdDevWidth,
            AvgBarrier,
            PiecesInGoal,
            PiecesInHome,
            PiecesAlmost,
            PiecesMax
        };

        template<int NR, int NC, typename state_t>
        void getFeatures( double* feature_vec, const state_t & s ){
            const int numFeatures = 18;

            // variables to store useful information for features
            int numPieces[2] = {0}, numBarriers[2] = {0};
            int numPiecesGoal[2] = {0}, numPiecesAlmostGoal[2] = {0}, numPiecesHome[2] = {0};
            double maxDist[2] = {0};
            double meanDist[2] = {0}, stdDist[2] = {0};
            double meanW[2] = {0}, stdW[2] = {0};

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
                        maxDist[team1] = std::max(maxDist[team1],tv);
                        meanDist[team1] += tv;
                        stdDist[team1] += tv*tv;

                        tv = c * invnc;
                        meanW[team1] += tv;
                        stdW[team1] += tv*tv;

                        if( r == 0 ){ numPiecesHome[team1]++; }
                        else if( r == 1){ numPiecesAlmostGoal[team1]++; }
                        else if( r == NR-1 ){ numPiecesGoal[team1]++; }
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
                        tv = r * invnr;
                        maxDist[team2] = std::max(maxDist[team2],(1-tv));
                        meanDist[team2] += tv;
                        stdDist[team2] += tv*tv;

                        tv = c * invnc;
                        meanW[team2] += tv;
                        stdW[team2] += tv*tv;

                        if( r == 0 ){ numPiecesGoal[team2]++; }
                        else if( r == NR-2 ){ numPiecesAlmostGoal[team2]++; }
                        else if( r == NR-1 ){ numPiecesHome[team2]++; }
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

            meanW[team1] /= static_cast<double>(numPieces[team1]);
            meanW[team2] /= static_cast<double>(numPieces[team2]);
            stdW[team1] = std::sqrt(stdW[team1]/static_cast<double>(numPieces[team1])
                                       - meanW[team1]*meanW[team1]);
            stdW[team2] = std::sqrt(stdW[team2]/static_cast<double>(numPieces[team2])
                                       - meanW[team2]*meanW[team2]);

            avgBarrierSize[team1] /= static_cast<double>(numBarriers[team1]);
            avgBarrierSize[team2] /= static_cast<double>(numBarriers[team2]);

            // set the feature vector
            feature_vec[Pieces1]        = numPieces[team1];
            feature_vec[MeanDist1]      = meanDist[team1];
            feature_vec[StdDevDist1]    = stdDist[team1];
            feature_vec[StdDevWidth1]   = stdW[team1];
            feature_vec[AvgBarrier1]    = avgBarrierSize[team1];
            feature_vec[Pieces1InGoal]  = numPiecesGoal[team1];
            feature_vec[Pieces1InHome]  = numPiecesHome[team1];
            feature_vec[Pieces1Almost]  = numPiecesAlmostGoal[team1];
            feature_vec[Pieces1Max]     = maxDist[team1];

            feature_vec[Pieces2]        = numPieces[team2];
            feature_vec[MeanDist2]      = meanDist[team2];
            feature_vec[StdDevDist2]    = stdDist[team2];
            feature_vec[StdDevWidth2]   = stdW[team2];
            feature_vec[AvgBarrier2]    = avgBarrierSize[team2];
            feature_vec[Pieces2InGoal]  = numPiecesGoal[team2];
            feature_vec[Pieces2InHome]  = numPiecesHome[team2];
            feature_vec[Pieces2Almost]  = numPiecesAlmostGoal[team2];
            feature_vec[Pieces2Max]     = maxDist[team2];
        }

        // defensive heuristic provided by the MP definition
        template<int NR = 8, int NC = 8,
                template<int,int> typename Rules = bt::baseline_rules>
        class defensive : public game::evaluator<Rules<NR,NC> >{
        public:
            typedef game::evaluator<Rules<NR,NC>> parent;
            typedef typename parent::state_t state_t;
            typedef typename parent::eval_t eval_t;


            defensive(){ generator.seed(17); }

            virtual eval_t utilityEstimate( const state_t & s, int team );
            virtual std::string name() const { return "Defensive Heuristics 2"; }

            void setCoefficient(int idx, double value ){ coefs[idx] = value; }
            int numCoefficients() const { return 15; }

        private:

            double coefs[15];

        };

        template<>
        typename defensive<8,8,bt::baseline_rules>::eval_t
        defensive<8,8,bt::baseline_rules>::utilityEstimate( const state_t & s, int team ){
            const int N = 9;
            double utility = 0.0, features[2*N] = {0.0};
            getFeatures<8,8,state_t>(features,s);
            int d1 = N*team;
            int d2 = N*((team+1)%2);
            double* f1 = &features[d1];
            double* f2 = &features[d2];
            utility = 2.0*f1[Pieces] - 50*f1[StdDevDist] - 50*f2[Pieces] + U(generator);
            return utility;
        }

        template<>
        typename defensive<5,10,bt::baseline_rules>::eval_t
        defensive<5,10,bt::baseline_rules>::utilityEstimate( const state_t & s, int team ){
            const int N = 9;
            double utility = 0.0, features[2*N] = {0.0};
            getFeatures<5,10,state_t>(features,s);
            int d1 = N*team;
            int d2 = N*((team+1)%2);
            double* f1 = &features[d1];
            double* f2 = &features[d2];
            utility = 2.0*f1[Pieces] - 20*f1[StdDevDist] - 50*f2[PiecesMax] + U(generator);
            return utility;
        }

        template<>
        typename defensive<8,8,bt::extended_rules>::eval_t
        defensive<8,8,bt::extended_rules>::utilityEstimate( const state_t & s, int team ){
            const int N = 9;
            double utility = coefs[0], features[2*N] = {0.0};
            getFeatures<8,8,state_t>(features,s);
            int d1 = N*team;
            int d2 = N*((team+1)%2);
            double* f1 = &features[d1];
            double* f2 = &features[d2];
            utility = 2.0*f1[Pieces]
                      - 20*f1[StdDevDist] - 10.0*f2[Pieces] - 100*f2[PiecesInGoal]
                      + U(generator);
            return utility;
        }




        // offensive heuristic provided by the MP definition
        template<int NR = 8, int NC = 8,
                template<int,int> typename Rules = bt::baseline_rules>
        class offensive : public game::evaluator<Rules<NR,NC>> {
        public:

            typedef game::evaluator<Rules<NR,NC>> parent;
            typedef typename parent::state_t state_t;
            typedef typename parent::eval_t eval_t;

            offensive(){ generator.seed(17); }

            virtual eval_t utilityEstimate( const state_t & s, int team );
            virtual std::string name() const { return "Offensive Heuristics 2"; }

            void setCoefficient(int idx, double value ){ coefs[idx] = value; }
            int numCoefficients() const { return 15; }

        private:

            double coefs[15];
        };

        template<>
        typename offensive<8,8,bt::baseline_rules>::eval_t
        offensive<8,8,bt::baseline_rules>::utilityEstimate( const state_t & s, int team ){
            const int N = 9;
            double utility = 0.0, features[2*N] = {0.0};
            getFeatures<8,8,state_t>(features,s);
            int d1 = N*team;
            int d2 = N*((team+1)%2);
            double* f1 = &features[d1];
            double* f2 = &features[d2];
            int opponent = team - 1 < 0 ? 1 : 0;
            utility = 100*f1[PiecesInGoal] + 0*f1[PiecesAlmost]
                      - 0*std::abs(f1[PiecesMax] - opponent)
                      - 20*(f1[StdDevDist]+f1[StdDevWidth]) - 100.0*f2[Pieces]
                      + U(generator);
            return utility;
        }

        template<>
        typename offensive<5,10,bt::baseline_rules>::eval_t
        offensive<5,10,bt::baseline_rules>::utilityEstimate( const state_t & s, int team ){
            const int N = 9;
            double utility = 0, features[2*N] = {0.0};
            getFeatures<5,10,state_t>(features,s);
            int d1 = N*team;
            int d2 = N*((team+1)%2);
            double* f1 = &features[d1];
            double* f2 = &features[d2];
            int opponent = team - 1 < 0 ? 1 : 0;
            utility = 100*f1[PiecesInGoal] + 0*f1[PiecesAlmost]
                      - 0*std::abs(f1[PiecesMax] - opponent)
                      - 10*(f1[StdDevDist]+f1[StdDevWidth]) - 100.0*f2[Pieces]
                      + U(generator);
            //utility = 60 - 2.0*f2[Pieces];
            return utility;
        }

        template<>
        typename offensive<8,8,bt::extended_rules>::eval_t
        offensive<8,8,bt::extended_rules>::utilityEstimate( const state_t & s, int team ){
            const int N = 9;
            double utility = 0, features[2*N] = {0.0};
            getFeatures<8,8,state_t>(features,s);
            int d1 = N*team;
            int d2 = N*((team+1)%2);
            double* f1 = &features[d1];
            double* f2 = &features[d2];
            int opponent = team - 1 < 0 ? 1 : 0;
            utility = 100*f1[PiecesInGoal] + 10*f1[PiecesAlmost]
                      + 0*std::abs(f1[MeanDist] - opponent)
                      - 50*f1[StdDevDist] - 100.0*f2[Pieces]
                      + U(generator);
            return utility;
        }
    }
}

#endif //SRC_BREAKTHROUGH_HANDCRAFT_HEURISTICS_HPP

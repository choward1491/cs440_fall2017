//
// Created by Christian J Howard on 10/28/17.
//

#ifndef SRC_BREAKTHROUGH_LEARNED_HEURISTICS_HPP_HPP
#define SRC_BREAKTHROUGH_LEARNED_HEURISTICS_HPP_HPP

#include "game_agent.hpp"
#include "breakthrough_rules.hpp"
#include "breakthrough_extended_rules.hpp"
#include <cmath>
#include <algorithm>

namespace bt {
    namespace learned {

        enum feature_t {
            Pieces1 = 0,
            MeanDist1,
            StdDevDist1,
            AvgBarrier1,
            Pieces1InGoal,
            Pieces1InHome,
            Pieces1Almost,
            Pieces2,
            MeanDist2,
            StdDevDist2,
            AvgBarrier2,
            Pieces2InGoal,
            Pieces2InHome,
            Pieces2Almost
        };

        template<int NR, int NC, typename state_t>
        void getFeatures( double* feature_vec, const state_t & s ){
            const int numFeatures = 14;

            // variables to store useful information for features
            int numPieces[2] = {0}, numBarriers[2] = {0};
            int numPiecesGoal[2] = {0}, numPiecesAlmostGoal[2] = {0}, numPiecesHome[2] = {0};
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
                        tv = (NR - r - 1) * invnr;
                        meanDist[team2] += tv;
                        stdDist[team2] += tv*tv;
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
            avgBarrierSize[team1] /= static_cast<double>(numBarriers[team1]);
            avgBarrierSize[team2] /= static_cast<double>(numBarriers[team2]);

            // set the feature vector
            feature_vec[Pieces1]        = numPieces[team1];
            feature_vec[MeanDist1]      = meanDist[team1];
            feature_vec[StdDevDist1]    = stdDist[team1];
            feature_vec[AvgBarrier1]    = avgBarrierSize[team1];
            feature_vec[Pieces1InGoal]  = numPiecesGoal[team1];
            feature_vec[Pieces1InHome]  = numPiecesHome[team1];
            feature_vec[Pieces1Almost]  = numPiecesAlmostGoal[team1];

            feature_vec[Pieces2]        = numPieces[team2];
            feature_vec[MeanDist2]      = meanDist[team2];
            feature_vec[StdDevDist2]    = stdDist[team2];
            feature_vec[AvgBarrier2]    = avgBarrierSize[team2];
            feature_vec[Pieces2InGoal]  = numPiecesGoal[team2];
            feature_vec[Pieces2InHome]  = numPiecesHome[team2];
            feature_vec[Pieces2Almost]  = numPiecesAlmostGoal[team2];
        }

        // defensive heuristic provided by the MP definition
        template<int NR = 8, int NC = 8,
                template<int,int> typename Rules = bt::baseline_rules>
        class defensive : public game::evaluator<Rules<NR,NC> >{
        public:
            typedef game::evaluator<Rules<NR,NC>> parent;
            typedef typename parent::state_t state_t;
            typedef typename parent::eval_t eval_t;


            defensive();

            virtual std::string name() const { return "Defensive Heuristics 2"; }
            virtual eval_t utilityEstimate( const state_t & s, int team ){
                const int N = 7;
                double utility = coefs[0], features[2*N] = {0.0};
                getFeatures<NR,NC,state_t>(features,s);
                int d1 = N*team;
                int d2 = N*((team+1)%2);
                double* f1 = &features[d1];
                double* f2 = &features[d2];
                for(int i = 0; i < N; ++i){ utility += (coefs[1+i]*f1[i] + coefs[1+N+i]*f2[i]); }
                return utility;
            }

            void setCoefficient(int idx, double value ){ coefs[idx] = value; }
            int numCoefficients() const { return 15; }

        private:

            double coefs[15];

        };

        template<>
        defensive<8,8,bt::baseline_rules>::defensive(){
            // place to specify coefficient values for training
            // via some optimization algorithm
            std::vector<double> values{59.939448, 90.290037, 77.960057, 27.668301, -55.565314,
                                       59.075213, 93.351355, -2.669065, -2.191130, -100.000000,
                                       -99.220344, 18.349018, -55.175475, -44.249640, -18.005058};

            for (int i = 0; i < values.size(); ++i) { coefs[i] = values[i]; }
        }

        template<>
        defensive<5,10,bt::baseline_rules>::defensive(){
            // place to specify coefficient values for training
            // via some optimization algorithm
            std::vector<double> values{93.767809, 93.445915, 45.903635, 26.950024, 2.747143,
                                       44.775770, 4.630077, -19.372543, -70.625754, -100.000000,
                                       38.720460, -5.559336, 55.754503, -16.739796, -1.328566};
            for (int i = 0; i < values.size(); ++i) { coefs[i] = values[i]; }
        }

        template<>
        defensive<8,8,bt::extended_rules>::defensive(){
            // place to specify coefficient values for training
            // via some optimization algorithm
            std::vector<double> values{-37.649863, -37.144878, 9.872399, 12.220168, 35.403425,
                                       32.751187, 1.078876, 93.662330, 6.569545, -55.120768,
                                       7.197839, 3.790219, -14.692976};
            for (int i = 0; i < values.size(); ++i) { coefs[i] = values[i]; }
        }




        // offensive heuristic provided by the MP definition
        template<int NR = 8, int NC = 8,
                template<int,int> typename Rules = bt::baseline_rules>
        class offensive : public game::evaluator<Rules<NR,NC>> {
        public:

            typedef game::evaluator<Rules<NR,NC>> parent;
            typedef typename parent::state_t state_t;
            typedef typename parent::eval_t eval_t;

            offensive();

            virtual std::string name() const { return "Offensive Heuristics 2"; }
            virtual eval_t utilityEstimate( const state_t & s, int team ){
                const int N = 7;
                double utility = coefs[0], features[2*N] = {0.0};
                getFeatures<NR,NC,state_t>(features,s);
                int d1 = N*team;
                int d2 = N*((team+1)%2);
                double* f1 = &features[d1];
                double* f2 = &features[d2];
                for(int i = 0; i < N; ++i){ utility += (coefs[1+i]*f1[i] + coefs[1+N+i]*f2[i]); }
                return utility;
            }

            void setCoefficient(int idx, double value ){ coefs[idx] = value; }
            int numCoefficients() const { return 15; }

        private:

            double coefs[15];
        };

        template<>
        offensive<8,8,bt::baseline_rules>::offensive(){
            // place to specify coefficient values for training
            // via some optimization algorithm
            std::vector<double> values{-100.000000, 40.302898, 16.032058, -50.924189, -2.850118,
                                       67.479011, -14.159257, -100.000000, -22.158032, 5.310339,
                                       -48.294387, 62.885468, 23.657457, 9.835322, -6.849803};
            for (int i = 0; i < values.size(); ++i) { coefs[i] = values[i]; }
        }

        template<>
        offensive<5,10,bt::baseline_rules>::offensive(){
            // place to specify coefficient values for training
            // via some optimization algorithm
            std::vector<double> values{-100.000000, 95.621605, 18.949259, -99.041519, 0.044186,
                                       -19.937606, 57.287289, -92.232369, -61.344184, -53.504748,
                                       -41.049456, -31.538069, -58.726937, 97.742120, -17.973088};
            for (int i = 0; i < values.size(); ++i) { coefs[i] = values[i]; }
        }

        template<>
        offensive<8,8,bt::extended_rules>::offensive(){
            // place to specify coefficient values for training
            // via some optimization algorithm
            std::vector<double> values{56.920636, -100.000000, 51.284370, 50.579308, -26.224259,
                                       -44.776577, 42.248693, 48.892484, 9.913991, -73.379589,
                                       -46.505328, 100.000000, 98.772487};
            for (int i = 0; i < values.size(); ++i) { coefs[i] = values[i]; }
        }
    }
}

#endif //SRC_BREAKTHROUGH_LEARNED_HEURISTICS_HPP_HPP

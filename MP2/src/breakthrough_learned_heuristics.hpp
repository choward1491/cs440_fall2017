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
            Pieces2,
            MeanDist2,
            StdDevDist2,
            AvgBarrier2,
            Pieces2InGoal,
            Pieces2InHome,
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
            const int numFeatures = 12;

            // variables to store useful information for features
            int numPieces[2] = {0}, numBarriers[2] = {0};
            int numPiecesGoal[2] = {0}, numPiecesHome[2] = {0};
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
            feature_vec[Pieces2]        = numPieces[team2];
            feature_vec[MeanDist2]      = meanDist[team2];
            feature_vec[StdDevDist2]    = stdDist[team2];
            feature_vec[AvgBarrier2]    = avgBarrierSize[team2];
            feature_vec[Pieces2InGoal]  = numPiecesGoal[team2];
            feature_vec[Pieces2InHome]  = numPiecesHome[team2];
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

            virtual eval_t utilityEstimate( const state_t & s, int team ){
                double utility = coefs[0], features[12] = {0.0};
                getFeatures<NR,NC,state_t>(features,s);
                int d1 = 6*team;
                int d2 = 6*((team+1)%2);
                double* f1 = &features[d1];
                double* f2 = &features[d2];
                for(int i = 0; i < 6; ++i){ utility += (coefs[1+d1+i]*f1[i] + coefs[1+d2+i]*f2[i]); }
                return utility;
            }

            void setCoefficient(int idx, double value ){ coefs[idx] = value; }
            int numCoefficients() const { return 13; }

        private:

            double coefs[13];

        };

        template<>
        defensive<8,8,bt::baseline_rules>::defensive(){
            // place to specify coefficient values for training
            // via some optimization algorithm
            std::vector<double> values{-8.438238, -62.449733, 37.233679, 41.052877, 26.838459,
                                       100.000000, -14.954589, 44.555133, -29.786572, -19.200262,
                                       -81.270441, 50.560994, 83.499648};
            for (int i = 0; i < values.size(); ++i) { coefs[i] = values[i]; }
        }

        template<>
        defensive<5,10,bt::baseline_rules>::defensive(){
            // place to specify coefficient values for training
            // via some optimization algorithm
            std::vector<double> values{73.748279, -71.315236, 14.795486, 58.455757, -5.976192,
                                       -24.939519, -5.069100, 91.270803, 36.565936, -35.422453,
                                       12.725626, -3.771359, -21.700270};
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

            virtual eval_t utilityEstimate( const state_t & s, int team ){
                double utility = coefs[0], features[12] = {0.0};
                getFeatures<NR,NC,state_t>(features,s);
                int d1 = 6*team;
                int d2 = 6*((team+1)%2);
                double* f1 = &features[d1];
                double* f2 = &features[d2];
                for(int i = 0; i < 6; ++i){ utility += (coefs[1+d1+i]*f1[i] + coefs[1+d2+i]*f2[i]); }
                return utility;
            }

            void setCoefficient(int idx, double value ){ coefs[idx] = value; }
            int numCoefficients() const { return 13; }

        private:

            double coefs[13];
        };

        template<>
        offensive<8,8,bt::baseline_rules>::offensive(){
            // place to specify coefficient values for training
            // via some optimization algorithm
            std::vector<double> values{11.445962, -80.638975, 79.559753, 87.647953, -3.245963,
                                       -10.052899, 88.242197, 5.039622, -2.314645, -32.941403,
                                       -93.925248, 20.782016, 19.502144};
            for (int i = 0; i < values.size(); ++i) { coefs[i] = values[i]; }
        }

        template<>
        offensive<5,10,bt::baseline_rules>::offensive(){
            // place to specify coefficient values for training
            // via some optimization algorithm
            std::vector<double> values{43.092594, 12.118083, -43.216656, -4.666741, 9.937200,
                                       33.517220, 4.101216, 50.988403, -0.922866, 50.918149,
                                       -43.044770, -70.381146, 100.000000};
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

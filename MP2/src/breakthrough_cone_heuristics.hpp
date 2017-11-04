//
// Created by Christian J Howard on 10/24/17.
//

#ifndef SRC_BREAKTHROUGH_CONE_HEURISTICS_HPP
#define SRC_BREAKTHROUGH_CONE_HEURISTICS_HPP

#include "game_agent.hpp"
#include "breakthrough_rules.hpp"
#include <cmath>
#include <algorithm>

namespace bt {
    namespace cone {

        double sign( double x ){
            return x < 0 ? -1.0 : 1.0;
        }

        double norm( std::pair<double,double> x ){
            return std::sqrt(x.first*x.first + x.second*x.second);
        }

        double inner_prod( std::pair<double,double> x1, std::pair<double,double> x2 ){
            return x1.first*x2.first + x1.second*x2.second;
        }

        double cross_e3( std::pair<double,double> x1, std::pair<double,double> x2 ){
            return x1.first*x2.second + x1.second*x2.first;
        }

        std::pair<double,double> diff(std::pair<double,double> X, std::pair<double,double> O){
            std::pair<double,double> out = X;
            out.first   -= O.first;
            out.second  -= O.second;
            return out;
        };

        double coneCost( std::pair<double,double> X, std::pair<double,double> O ){
            std::pair<double,double> d1(-1.0/std::sqrt(2),1.0/std::sqrt(2));
            std::pair<double,double> d2(1.0/std::sqrt(2),1.0/std::sqrt(2));
            std::pair<double,double> delta = diff(X,O);
            double dmag = norm(delta);
            double c    = sign(inner_prod(d1,delta));
            double s    = -c*sign(cross_e3(d1,delta))*sign(cross_e3(d2,delta))/dmag;
            return std::min<double>(0,s);
        }


        // defensive heuristic provided by the MP definition
        template<int NR = 8, int NC = 8>
        class defensive : public game::evaluator<bt::baseline_rules<NR,NC>>{
        public:
            typedef game::evaluator<bt::baseline_rules<NR,NC>> parent;
            typedef typename parent::state_t state_t;
            typedef typename parent::eval_t eval_t;


            defensive(){ }

            virtual eval_t utilityEstimate( const state_t & s, int team ){
                enum teams: int { team1 = 0, team2 };
                int numTeam2inTeam1Base = 0, numTeam1inTeam2Base = 0;
                int num1 = 0, num2 = 0;
                int nr = s.numRows()-1;

                // gather useful data from state
                for(int r = 0; r < s.numRows(); ++r){
                    for(int c = 0; c < s.numCols(); ++c){
                        if( s.getStateAt(r,c) == team2 ){
                            if( r == 0 ){ ++numTeam2inTeam1Base; }
                            team1_pos[num1++] = c + NC*r;
                        }
                        else if( s.getStateAt(r,c) == team1) {
                            if( r == nr ) { ++numTeam1inTeam2Base; }
                            team2_pos[num2++] = c + NC*r;
                        }
                    }// end for c
                }// end for r

                // compute heuristic function
                double gamma = 5.0;
                double dN = numTeam1inTeam2Base - numTeam2inTeam1Base;
                double H = 1e5 * static_cast<double>(dN*(team == team1) - dN*(team == team2));
                int *X = team1_pos, *O = team2_pos;
                int *numx = &num1, *numo = &num2;
                if( team == team2 ){
                    O = team1_pos; X = team2_pos;
                    numx = &num2; numo = &num1;
                }

                for(int i = 0; i < *numx; ++i){
                    std::pair<double,double> x(X[i]/NC,X[i]%NC);
                    double tmp = teamDist(x,team), min_cc = std::numeric_limits<eval_t>::max();
                    for(int j = 0; j < *numo; ++j){
                        std::pair<double,double> o(O[i]/NC,O[i]%NC);
                        min_cc = std::min(min_cc,coneCost(x,o));
                    }// end for j
                    H += (tmp - gamma*min_cc);
                }// end for i

                return H/static_cast<double>(1 + 10 * *numo);
            }

        private:
            int team1_pos[NR*NC], team2_pos[NR*NC];

            double teamDist(std::pair<double,double> x, int team){
                if( team == 0 ){
                    return (NR-1) - x.first;
                }else{
                    return x.first;
                }
            }

        };


        // offensive heuristic provided by the MP definition
        template<int NR = 8, int NC = 8>
        class offensive : public game::evaluator<bt::baseline_rules<NR,NC>>{
        public:

            typedef game::evaluator<bt::baseline_rules<NR,NC>> parent;
            typedef typename parent::state_t state_t;
            typedef typename parent::eval_t eval_t;

            offensive(){ }

            virtual eval_t utilityEstimate( const state_t & s, int team ){
                enum teams: int { team1 = 0, team2 };
                int numTeam2inTeam1Base = 0, numTeam1inTeam2Base = 0;
                int num1 = 0, num2 = 0;
                int nr = s.numRows()-1;

                // gather useful data from state
                for(int r = 0; r < s.numRows(); ++r){
                    for(int c = 0; c < s.numCols(); ++c){
                        if( s.getStateAt(r,c) == team2 ){
                            if( r == 0 ){ ++numTeam2inTeam1Base; }
                            team1_pos[num1++] = c + NC*r;
                        }
                        else if( s.getStateAt(r,c) == team1) {
                            if( r == nr ) { ++numTeam1inTeam2Base; }
                            team2_pos[num2++] = c + NC*r;
                        }
                    }// end for c
                }// end for r

                // compute heuristic function
                double gamma = 5.0;
                double H = 1e3 * static_cast<double>(numTeam1inTeam2Base*(team == team1) + numTeam2inTeam1Base*(team == team2));
                int *X = team1_pos, *O = team2_pos;
                int *numx = &num1, *numo = &num2;
                if( team == team2 ){
                    O = team1_pos; X = team2_pos;
                    numx = &num2; numo = &num1;
                }

                for(int i = 0; i < *numx; ++i){
                    std::pair<double,double> x(X[i]/NC,X[i]%NC);
                    double tmp = teamDist(x,team), min_cc = std::numeric_limits<eval_t>::max();
                    for(int j = 0; j < *numo; ++j){
                        std::pair<double,double> o(O[i]/NC,O[i]%NC);
                        min_cc = std::min(min_cc,coneCost(x,o));
                    }// end for j
                    H += (tmp + gamma*min_cc);
                }// end for i

                return H/static_cast<double>(1 + 10 * *numo);
            }

        private:
            int team1_pos[NR*NC], team2_pos[NR*NC];

            double teamDist(std::pair<double,double> x, int team){
                if( team == 0 ){
                    return (NR-1) - x.first;
                }else{
                    return x.first;
                }
            }
        };
    }
}

#endif //SRC_BREAKTHROUGH_CONE_HEURISTICS_HPP

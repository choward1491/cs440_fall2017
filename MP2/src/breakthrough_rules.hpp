//
// Created by Christian J Howard on 10/21/17.
//

#ifndef SRC_BREAKTHROUGH_RULES_HPP
#define SRC_BREAKTHROUGH_RULES_HPP

#include "breakthrough_state.hpp"
#include "breakthrough_actions.hpp"
#include "breakthrough_transition_func.hpp"
#include "breakthrough_piece_type.hpp"
#include <limits>
#include <set>

namespace bt {

    template<int NR = 8, int NC = 8>
    class baseline_rules {
    public:

        // important typedefs for metaprogramming stuff
        typedef state<NR,NC>            state_t;
        typedef action_t                action_t;
        typedef transition<state_t>     transition_t;
        typedef std::set<int>           actions;

        bool isGameComplete( const state_t & s ){
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


            return (teamCount[team1] == 0) || (teamCount[team2] == 0)
                   || (numTeam1inTeam2Base != 0) || (numTeam2inTeam1Base != 0);
        }

        template<typename num_type>
        num_type evalFinalUtility( const state_t & s, int team){
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

            bool moreTeam2 = numTeam2inTeam1Base > numTeam1inTeam2Base;
            if( moreTeam2 ){
                switch(team){
                    case team1: return std::numeric_limits<num_type>::lowest();
                    case team2: return std::numeric_limits<num_type>::max();
                }
            }else{
                switch(team){
                    case team1: return std::numeric_limits<num_type>::max();
                    case team2: return std::numeric_limits<num_type>::lowest();
                }
            }
        }

        void getValidActionSet( const state_t & s, int team_value, actions & action_set ){

            // clear action set in case any old elements remain
            action_set.clear();

            // define transition model
            transition_t F;

            // get the number of rows and columns in grid
            int nr = state_t::numRows(), nc = state_t::numCols();
            int nd = nr*nc;
            int other_team = (team_value - 1) < 0 ? 1 : 0;

            // loop through grid and add any valid moves
            for (int i = 0; i < nr; ++i) {
                for (int j = 0; j < nc; ++j) {
                    int k = s.hash(i,j);

                    if( notAtEnd(i,team_value,nr) && s.getStateAt(i,j) == team_value ){

                        // try to add action for forward move
                        int a1 = actionHash(k,Forward,nd);
                        if( F.getNodeTypeAtFuturePos(s, a1) == None ){
                            action_set.insert( a1 );
                        }

                        // try to add action for left move
                        int a = actionHash(k,LeftDiagonal,nd);
                        if( j != 0 &&
                            ( F.getNodeTypeAtFuturePos(s,a) == None ||
                            ( F.getNodeTypeAtFuturePos(s,a) == other_team
                            && F.getNodeTypeAtFuturePos(s,a1) != other_team )) )
                        {
                            action_set.insert( a );
                        }

                        // try to add action for right move
                        a = actionHash(k,RightDiagonal,nd);
                        if( j != (nc-1) &&
                             ( F.getNodeTypeAtFuturePos(s,a) == None ||
                               ( F.getNodeTypeAtFuturePos(s,a) == other_team
                                 && F.getNodeTypeAtFuturePos(s,a1) != other_team )))
                        {
                            action_set.insert( a );
                        }

                    }
                }// end for j
            }// end for i

        }

    private:

        bool notAtEnd(int row, int team_value, int num_rows ){
            if( team_value != 0 ){
                return row != 0;
            }else{
                return row != (num_rows-1);
            }
        }

    };
}

#endif //SRC_BREAKTHROUGH_RULES_HPP

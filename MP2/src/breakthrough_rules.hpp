//
// Created by Christian J Howard on 10/21/17.
//

#ifndef SRC_BREAKTHROUGH_RULES_HPP
#define SRC_BREAKTHROUGH_RULES_HPP

#include "breakthrough_state.hpp"
#include "breakthrough_actions.hpp"
#include "breakthrough_transition_func.hpp"
#include "breakthrough_piece_type.hpp"
#include <set>

namespace bt {

    class baseline_rules {
    public:

        // important typedefs for metaprogramming stuff
        typedef state       state_t;
        typedef action_t    action_t;
        typedef transition  transition_t;

        bool isGameComplete( const state & s ){
            return false;
        }

        double evalFinalUtility( const state & s ){
            return 1.0;
        }

        void getValidActionSet( const state & s, int team_value, std::set<int> & action_set ){

            // clear action set in case any old elements remain
            action_set.clear();

            // define transition model
            transition<state> F;

            // get the number of rows and columns in grid
            int nr = state::numRows(), nc = state::numCols();
            int nd = nr*nc;

            // loop through grid and add any valid moves
            for (int i = 0; i < nr; ++i) {
                for (int j = 0; j < nc; ++j) {
                    int k = s.hash(i,j);

                    if( notAtEnd(i,team_value,nr) && s.getStateAt(i,j) == team_value ){

                        // try to add action for forward move
                        int a = actionHash(k,Forward,nd);
                        if( F.getNodeTypeAtFuturePos(s, a) == None ){
                            action_set.insert( a );
                        }

                        // try to add action for left move
                        a = actionHash(k,LeftDiagonal,nd);
                        if( j != 0 && F.getNodeTypeAtFuturePos(s,a) != team_value ){
                            action_set.insert( a );
                        }

                        // try to add action for right move
                        a = actionHash(k,RightDiagonal,nd);
                        if( j != (nc-1) && F.getNodeTypeAtFuturePos(s,a) != team_value ){
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

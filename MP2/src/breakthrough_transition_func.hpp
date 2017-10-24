//
// Created by Christian J Howard on 10/21/17.
//

#ifndef SRC_BREAKTHROUGH_TRANSITION_FUNC_HPP
#define SRC_BREAKTHROUGH_TRANSITION_FUNC_HPP

#include "breakthrough_state.hpp"
#include "breakthrough_actions.hpp"
#include "breakthrough_piece_type.hpp"

namespace bt {

    template<typename state>
    class transition {
    public:

        int getNodeTypeAtFuturePos(const state & s, int action){
            int k = action % state::numStateDims();
            std::pair<int,int> rc = state::invhash(k);
            int r = rc.first, c = rc.second;
            int move = action / state::numStateDims();

            int team = s.getStateAt(k);
            int d = 1 - 2*team;

            switch(move){
                case Forward:       return s.getStateAt(r + d, c);
                case LeftDiagonal:  return s.getStateAt(r + d, c-1);
                case RightDiagonal: return s.getStateAt(r + d, c+1);
            }
        }

        state operator()(const state & s, int action ){
            int k = action % state::numStateDims();
            std::pair<int,int> rc = state::invhash(k);
            int r = rc.first, c = rc.second;
            int move = action / state::numStateDims();
            state sn = s;

            int team = s.getStateAt(k);
            int d = 1 - 2*team;

            switch(move){
                case Forward:       sn.setStateAt(r + d, c, team);  break;
                case LeftDiagonal:  sn.setStateAt(r + d, c-1, team);break;
                case RightDiagonal: sn.setStateAt(r + d, c+1, team);break;
            }

            sn.setStateAt(k,piece_t::None);

            return sn;
        }

        void operator()(const state & s, int action, state & sn ){
            int k = action % state::numStateDims();
            std::pair<int,int> rc = state::invhash(k);
            int r = rc.first, c = rc.second;
            int move = action / state::numStateDims();
            sn = s;

            int team = s.getStateAt(k);
            int d = 1 - 2*team;

            switch(move){
                case Forward:       sn.setStateAt(r + d, c, team);  break;
                case LeftDiagonal:  sn.setStateAt(r + d, c-1, team);break;
                case RightDiagonal: sn.setStateAt(r + d, c+1, team);break;
            }

            sn.setStateAt(k,piece_t::None);
        }
    };

}


#endif //SRC_BREAKTHROUGH_TRANSITION_FUNC_HPP

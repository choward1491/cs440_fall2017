//
// Created by Christian J Howard on 10/21/17.
//

#ifndef SRC_BREAKTHROUGH_TRANSITION_FUNC_HPP
#define SRC_BREAKTHROUGH_TRANSITION_FUNC_HPP

#include "breakthrough_state.hpp"
#include "breakthrough_actions.hpp"

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
                case Forward:       return s.getActionAt(r + d, c, team);
                case LeftDiagonal:  return s.getActionAt(r + d, c-1, team);
                case RightDiagonal: return s.getActionAt(r + d, c+1, team);
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
                case Forward:       sn.setActionAt(r + d, c, team);  break;
                case LeftDiagonal:  sn.setActionAt(r + d, c-1, team);break;
                case RightDiagonal: sn.setActionAt(r + d, c+1, team);break;
            }

            sn.setActionAt(k,state::None);

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
                case Forward:       sn.setActionAt(r + d, c, team);  break;
                case LeftDiagonal:  sn.setActionAt(r + d, c-1, team);break;
                case RightDiagonal: sn.setActionAt(r + d, c+1, team);break;
            }

            sn.setActionAt(k,state::None);

            return sn;
        }
    };

}


#endif //SRC_BREAKTHROUGH_TRANSITION_FUNC_HPP

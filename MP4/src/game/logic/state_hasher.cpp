/*
 *  state_hasher.cpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#include "state_hasher.hpp"

namespace pong {
    extern const double height           = 1.0;
    extern const double width            = 1.0;
    extern const double paddle_height    = 0.2;
    mdp_t::state_type componentHasher(size_t vidx, const std::vector<double> & state, const std::vector<std::vector<double>> & var_values) {
        switch(vidx){
            case RL::Ball_x:        return algos::binary_search(state[vidx], &var_values[vidx][0], var_values[vidx].size());
            case RL::Ball_y:        return algos::binary_search(state[vidx], &var_values[vidx][0], var_values[vidx].size());
            case RL::BallSpeed_x:   return (state[vidx] > 0);
            case RL::BallSpeed_y: {
                if( std::abs(state[vidx]) < 0.015 ){ return 1; }
                return (state[vidx] > 0) ? 2 : 0;
            }
            case RL::FriendlyPaddle_y: {
                if( state[vidx] >= (height - paddle_height) ){ return 11; }
                return (12 * state[vidx] / (height - paddle_height));
            }
            case RL::OpponentPaddle_y: {
                if( state[vidx] >= (height - paddle_height) ){ return 11; }
                return (12 * state[vidx] / (height - paddle_height));
            }
            default:                return 0;
        }
    }
    mdp_t::state_type getStateHash( const std::vector<double> & state, const std::vector<std::vector<double>> & var_values ) {
        mdp_t::state_type hash = 2;
        mdp_t::state_type factor = 1;
        for(size_t idx = 0; idx < state.size(); idx++){
            hash += factor*componentHasher(idx,state,var_values);
            factor *= var_values[idx].size();
        }
        return hash;
    }
}

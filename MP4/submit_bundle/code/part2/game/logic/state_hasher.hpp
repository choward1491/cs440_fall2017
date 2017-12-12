/*
 *  state_hasher.hpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#ifndef state_hasher_hpp
#define state_hasher_hpp

#include "../../RL/pong_mdp.hpp"

namespace pong {
    typedef RL::mdp<>   mdp_t;
    extern const double height, width, paddle_height;
    mdp_t::state_type componentHasher(size_t vidx, const std::vector<double> & state, const std::vector<std::vector<double>> & var_values);
    mdp_t::state_type getStateHash( const std::vector<double> & state, const std::vector<std::vector<double>> & var_values );
}

#endif /* state_hasher_hpp */

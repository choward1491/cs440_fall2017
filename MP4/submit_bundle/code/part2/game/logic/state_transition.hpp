/*
 *  state_transition.hpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#ifndef state_transition_hpp
#define state_transition_hpp

#include <vector>

namespace pong {
    size_t state_transition( std::vector<double> & state,
                            double newPlayer1PaddlePos, double newPlayer2PaddlePos,
                            bool player1IsWall, bool player2IsWall);
}

#endif /* state_transition_hpp */

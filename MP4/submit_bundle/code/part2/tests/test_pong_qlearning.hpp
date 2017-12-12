/*
 *  test_pong_qlearning.hpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/7/17
 *  Purpose:
 *
 */

#ifndef test_pong_qlearning_hpp
#define test_pong_qlearning_hpp

namespace test {
    void pongQLearningSingle( unsigned int num_episodes, double eps = 1e-1, double discountFactor = 0.5 );
    void pongQLearningSingleActualPlay();
    void pongQLearningOpponent( unsigned int num_episodes, double eps = 1e-1, double discountFactor = 0.5 );
    void pongQLearningOpponentActualPlay(unsigned int num_episodes, double eps = 1e-1, double discountFactor = 0.5);
    void pongQLearningOpponentLargeInit();
}

#endif /* test_pong_qlearning_hpp */

/*
 *  test_pong_qlearning.cpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/7/17
 *  Purpose:
 *
 */

#include "test_pong_qlearning.hpp"
#include "../RL/pong_mdp.hpp"
#include "../RL/q_learning.hpp"
#include "../RL/learning_rates.hpp"

namespace test {
    void pongQLearningSingle() {
        typedef RL::pong::mdp<>             pong_mdp;
        typedef RL::q_learning<pong_mdp>    qlearner_t;
        
        std::string data_file = "/Users/cjh/Documents/pong_test_single.bin";
        
        qlearner_t ql;
        pong_mdp & pmdp = ql.getMDP();
        pmdp.setVariableValues(RL::pong::Ball_x, 0.0, 1.0, 12);
        pmdp.setVariableValues(RL::pong::Ball_y, 0.0, 1.0, 12);
        pmdp.setVariableValues(RL::pong::BallSpeed_x, {-1, 1});
        pmdp.setVariableValues(RL::pong::BallSpeed_y, {-1, 0, 1});
        pmdp.setVariableValues(RL::pong::FriendlyPaddle_y, 0.0, 1.0, 12);
        
        ql.setDiscountFactor(0.99);
        ql.setExplorationThreshold(1e-1);
        ql.init();
        ql.train<RL::state_tracker_rate,void (*)(void)>(10000000);
        ql.save(data_file);
    }
    
    void pongQLearningSingleActualPlay() {
        typedef RL::pong::mdp<>             pong_mdp;
        typedef RL::q_learning<pong_mdp>    qlearner_t;
        
        std::string data_file = "/Users/cjh/Documents/pong_test_single.bin";
        std::string data_file2 = "/Users/cjh/Documents/pong_test_single2.bin";
        
        qlearner_t ql;
        pong_mdp & pmdp = ql.getMDP();
        pmdp.setVariableValues(RL::pong::Ball_x, 0.0, 1.0, 12);
        pmdp.setVariableValues(RL::pong::Ball_y, 0.0, 1.0, 12);
        pmdp.setVariableValues(RL::pong::BallSpeed_x, {-1, 1});
        pmdp.setVariableValues(RL::pong::BallSpeed_y, {-1, 0, 1});
        pmdp.setVariableValues(RL::pong::FriendlyPaddle_y, 0.0, 1.0, 12);
        
        ql.setDiscountFactor(0.99);
        ql.setExplorationThreshold(0);
        ql.init();
        ql.load(data_file);
        ql.train<RL::state_tracker_rate,void (*)(void)>(1000);
        ql.save(data_file2);
    }
    
    void pongQLearningOpponent() {
        typedef RL::pong::mdp<>             pong_mdp;
        typedef RL::q_learning<pong_mdp>    qlearner_t;
        
        std::string data_file = "/Users/cjh/Documents/pong_test_opp.bin";
        
        qlearner_t ql;
        pong_mdp & pmdp = ql.getMDP();
        pmdp.setVariableValues(RL::pong::Ball_x, 0.0, 1.0, 12);
        pmdp.setVariableValues(RL::pong::Ball_y, 0.0, 1.0, 12);
        pmdp.setVariableValues(RL::pong::BallSpeed_x, {-1, 1});
        pmdp.setVariableValues(RL::pong::BallSpeed_y, {-1, 0, 1});
        pmdp.setVariableValues(RL::pong::FriendlyPaddle_y, 0.0, 1.0, 12);
        pmdp.setVariableValues(RL::pong::OpponentPaddle_y, 0.0, 1.0, 12);
        
        ql.setDiscountFactor(0.99);
        ql.setExplorationThreshold(1e-1);
        ql.init();
        ql.train<RL::state_tracker_rate,void (*)(void)>(10);
        ql.save(data_file);
    }
}

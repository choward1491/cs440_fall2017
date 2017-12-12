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
    void pongQLearningSingle(unsigned int num_episodes, double eps, double discountFactor) {
        typedef pong::RL::mdp<>             pong_mdp;
        typedef RL::q_learning<pong_mdp>    qlearner_t;
        
        std::string data_file = "/Users/cjh/Documents/pong_test_single.bin";
        
        qlearner_t ql;
        pong_mdp & pmdp = ql.getMDP();
        pmdp.setVariableValues(pong::RL::Ball_x, 0.0, 1.0, 12);
        pmdp.setVariableValues(pong::RL::Ball_y, 0.0, 1.0, 12);
        pmdp.setVariableValues(pong::RL::BallSpeed_x, {-1, 1});
        pmdp.setVariableValues(pong::RL::BallSpeed_y, {-1, 0, 1});
        pmdp.setVariableValues(pong::RL::FriendlyPaddle_y, 0.0, 1.0, 12);
        
        ql.setDiscountFactor(discountFactor);
        ql.setExplorationThreshold(eps);
        ql.init();
        ql.train<RL::state_tracker_rate,void (*)(double)>(1e1,num_episodes);
        ql.save(data_file);
    }
    
    void pongQLearningSingleActualPlay() {
        typedef pong::RL::mdp<>             pong_mdp;
        typedef RL::q_learning<pong_mdp>    qlearner_t;
        
        std::string data_file = "/Users/cjh/Documents/pong_test_single.bin";
        std::string data_file2 = "/Users/cjh/Documents/pong_test_single2.bin";
        
        qlearner_t ql;
        pong_mdp & pmdp = ql.getMDP();
        pmdp.setVariableValues(pong::RL::Ball_x, 0.0, 1.0, 12);
        pmdp.setVariableValues(pong::RL::Ball_y, 0.0, 1.0, 12);
        pmdp.setVariableValues(pong::RL::BallSpeed_x, {-1, 1});
        pmdp.setVariableValues(pong::RL::BallSpeed_y, {-1, 0, 1});
        pmdp.setVariableValues(pong::RL::FriendlyPaddle_y, 0.0, 1.0, 12);
        
        ql.setDiscountFactor(0.99);
        ql.setExplorationThreshold(0);
        ql.init();
        ql.load(data_file);
        ql.train<RL::state_tracker_rate,void (*)(double)>(1e1,1000);
        ql.save(data_file2);
    }
    
    class PrintGameWins {
    public:
        
        PrintGameWins(){
            num_wins[0] = num_wins[1] = 0;
        }
        
        void operator()(double reward){
            if( reward == -1.0 ){
                ++num_wins[1];
                printf("P1 vs P2 | %u - %u \n",num_wins[0],num_wins[1]);
            }else if(reward == 1.0){
                ++num_wins[0];
                printf("P1 vs P2 | %u - %u \n",num_wins[0],num_wins[1]);
            }
        }
    private:
        unsigned int num_wins[2];
    };
    
    void pongQLearningOpponent(unsigned int num_episodes, double eps, double discountFactor) {
        typedef pong::RL::mdp<>             pong_mdp;
        typedef RL::q_learning<pong_mdp>    qlearner_t;
        
        std::string data_file = "/Users/cjh/Documents/pong_test_opp.bin";
        
        qlearner_t ql;
        pong_mdp & pmdp = ql.getMDP();
        pmdp.setVariableValues(pong::RL::Ball_x, 0.0, 1.0, 12);
        pmdp.setVariableValues(pong::RL::Ball_y, 0.0, 1.0, 12);
        pmdp.setVariableValues(pong::RL::BallSpeed_x, {-1, 1});
        pmdp.setVariableValues(pong::RL::BallSpeed_y, {-1, 0, 1});
        pmdp.setVariableValues(pong::RL::FriendlyPaddle_y, 0.0, 1.0, 12);
        pmdp.setVariableValues(pong::RL::OpponentPaddle_y, 0.0, 1.0, 12);
        
        ql.setDiscountFactor(discountFactor);
        ql.setExplorationThreshold(eps);
        ql.init();
        ql.train<RL::state_tracker_rate,PrintGameWins>(1e1,num_episodes);
        ql.save(data_file);
    }
    
    void pongQLearningOpponentActualPlay(unsigned int num_episodes, double eps, double discountFactor) {
        typedef pong::RL::mdp<>             pong_mdp;
        typedef RL::q_learning<pong_mdp>    qlearner_t;
        
        std::string data_file = "/Users/cjh/Documents/pong_test_opp.bin";
        std::string data_file2 = "/Users/cjh/Documents/pong_test_opp2.bin";
        
        PrintGameWins callback;
        qlearner_t ql;
        pong_mdp & pmdp = ql.getMDP();
        pmdp.setVariableValues(pong::RL::Ball_x, 0.0, 1.0, 12);
        pmdp.setVariableValues(pong::RL::Ball_y, 0.0, 1.0, 12);
        pmdp.setVariableValues(pong::RL::BallSpeed_x, {-1, 1});
        pmdp.setVariableValues(pong::RL::BallSpeed_y, {-1, 0, 1});
        pmdp.setVariableValues(pong::RL::FriendlyPaddle_y, 0.0, 1.0, 12);
        pmdp.setVariableValues(pong::RL::OpponentPaddle_y, 0.0, 1.0, 12);
        
        ql.setDiscountFactor(discountFactor);
        ql.setExplorationThreshold(eps);
        ql.init();
        ql.load(data_file);
        ql.train<RL::state_tracker_rate,PrintGameWins>(1e1,num_episodes,1e6,&callback);
        ql.save(data_file2);
    }
    
    void pongQLearningOpponentLargeInit() {
        typedef pong::RL::mdp<>             pong_mdp;
        typedef RL::q_learning<pong_mdp>    qlearner_t;
        
        std::string data_file = "/Users/cjh/Documents/pong_test_opp_li.bin";
        
        qlearner_t ql;
        pong_mdp & pmdp = ql.getMDP();
        pmdp.setVariableValues(pong::RL::Ball_x, 0.0, 1.0, 12);
        pmdp.setVariableValues(pong::RL::Ball_y, 0.0, 1.0, 12);
        pmdp.setVariableValues(pong::RL::BallSpeed_x, {-1, 1});
        pmdp.setVariableValues(pong::RL::BallSpeed_y, {-1, 0, 1});
        pmdp.setVariableValues(pong::RL::FriendlyPaddle_y, 0.0, 1.0, 12);
        pmdp.setVariableValues(pong::RL::OpponentPaddle_y, 0.0, 1.0, 12);
        
        ql.setDiscountFactor(0.70);
        ql.setExplorationThreshold(0.0);
        ql.init(3.0);
        ql.train<RL::state_tracker_rate,void (*)(double)>(1e1,10*1000000);
        ql.save(data_file);
    }
}

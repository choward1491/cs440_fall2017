/*
 *  pong_training.cpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/9/17
 *  Purpose:
 *
 */

#include "pong_training.hpp"
#include "pong_mdp.hpp"
#include "q_learning.hpp"
#include "learning_rates.hpp"

void pong::train( const parser::config & cparser ) {
    // define useful typedefs
    typedef pong::RL::mdp<>             pong_mdp;
    typedef ::RL::q_learning<pong_mdp>    qlearner_t;
    
    // set the default values for training
    unsigned int numEpisodes        = 1e6;
    unsigned int printFrequency     = 1e3;
    double discountFactor           = 0.9;
    double epsilonGreedyProbability = 1e-1;
    double alpha_value              = 1e1;
    std::string trainingType        = "wall";
    std::string outputQValuesFile   = "qvalues.bin";
    
    // update values if necessary
    if( cparser.exist("printFrequency") )           { printFrequency = cparser.retrieve<double>("printFrequency"); }
    if( cparser.exist("numEpisodes") )              { numEpisodes = cparser.retrieve<double>("numEpisodes"); }
    if( cparser.exist("discountFactor") )           { discountFactor = cparser.retrieve<double>("discountFactor"); }
    if( cparser.exist("alphaValue") )               { alpha_value = cparser.retrieve<double>("alphaValue"); }
    if( cparser.exist("epsilonGreedyProbability") ) { epsilonGreedyProbability = cparser.retrieve<double>("epsilonGreedyProbability"); }
    if( cparser.exist("trainingType") )             { trainingType = cparser.retrieve<std::string>("trainingType"); }
    if( cparser.exist("outputQValues") )            { outputQValuesFile = cparser.retrieve<std::string>("outputQValues"); }
    
    // do the training
    qlearner_t ql;
    pong_mdp & pmdp = ql.getMDP();
    pmdp.setVariableValues(pong::RL::Ball_x, 0.0, 1.0, 12);
    pmdp.setVariableValues(pong::RL::Ball_y, 0.0, 1.0, 12);
    pmdp.setVariableValues(pong::RL::BallSpeed_x, {-1, 1});
    pmdp.setVariableValues(pong::RL::BallSpeed_y, {-1, 0, 1});
    pmdp.setVariableValues(pong::RL::FriendlyPaddle_y, 0.0, 1.0, 12);
    if( trainingType == "opponent" ){
        pmdp.setVariableValues(pong::RL::OpponentPaddle_y, 0.0, 1.0, 12);
    }
    
    ql.setDiscountFactor(discountFactor);
    ql.setExplorationThreshold(epsilonGreedyProbability);
    if( cparser.exist("oldQValues") )   { ql.init(cparser.retrieve<std::string>("oldQValues")); }
    else                                { ql.init(); }
    ql.train<::RL::state_tracker_rate,void (*)(double)>(alpha_value,numEpisodes,printFrequency);
    ql.save(outputQValuesFile);
}

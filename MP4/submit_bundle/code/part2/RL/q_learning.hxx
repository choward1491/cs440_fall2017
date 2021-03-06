/*
 *  q_learning.hxx
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/7/17
 *  Purpose:
 *
 */

#ifndef q_learning_impl_hxx
#define q_learning_impl_hxx

// useful macros for defining class methods
#define HEADER template<typename mdp_t, typename num_t>
#define CLASS q_learning<mdp_t,num_t>

// standard library stuff

// template class interface
#include "q_learning.hpp"

// useful custom libraries
#include "../utility/binary_vec.hpp"

// implementation details

namespace RL {
    
    // ctor/dtor
    HEADER CLASS::q_learning():sampler(17),model(),gamma(0.9),
    q_values(),num_episodes_tot(0),epsilon(1e-1){
        
    }
    
    // getter for the model
    HEADER mdp_t & CLASS::getMDP() {
        return model;
    }
    HEADER const mdp_t & CLASS::getMDP() const {
        return model;
    }
    
    // main learning algorithm
    HEADER
    template<typename alpha_func, typename callback>
    void CLASS::train(num_t alpha_value, unsigned int num_episodes, unsigned int count_print_msg, callback* cback) {
        
        // define random number generators for use in training
        std::uniform_real_distribution<num_t>       U(0,1);
        std::uniform_int_distribution<action_type>  Ua(0,model.numActions()-1);
        
        // define the alpha function for use in defining the learning rate
        alpha_func  alphaf(model.numStates(),model.numActions(),alpha_value);
        num_t       alpha = 0.0;
        
        //loop through the number of games that will be used
        // to train
        num_t avg_reward = 0, tot_reward = 0;
        for(unsigned int n = 0; n < num_episodes; ++n){
            
            // reset the game to some initial configuration
            model.init();
            
            // play single episode until reach terminal state
            num_t       r = 0.0;
            state_type  s = 0, sn = 0;
            action_type a = 0;
            num_t tmp_avg_reward = 0;
            const num_t inv_factor = 1.0/count_print_msg;
            
            while( !model.episodeComplete() ){
                
                // get current state
                s = model.getState();
                
                // get next action
                num_t eps_scale = (1000.0/(1000.0 + n));
                if( U(sampler) < epsilon*eps_scale )    {   a = Ua(sampler);   }
                else                                    {   a = argmaxQ(s);    }
                
                // perform the action
                model.transition(a);
                
                // get the reward for this move and the new state
                r   = model.getLatestReward();
                sn  = model.getState();
                
                // do Q learning iteration
                alpha = alphaf(num_episodes_tot++, s, a);
                Q(s,a) = (1-alpha)*Q(s,a) + alpha*(r + gamma*maxQ(sn) );
                
                // update reward related statistics
                tot_reward      += r;
                tmp_avg_reward  += r*inv_factor;
                
                // if callback is defined
                if( cback ){
                    (*cback)(r); // may still refine this
                }
                
            }// end single game loop
            
            // print message if necessary
            if( n && (n % count_print_msg == 0) ){
                const num_t ratio = count_print_msg / static_cast<num_t>(n);
                avg_reward = ratio*tmp_avg_reward + (1.0 - ratio)*avg_reward;
                printf("Episode %10u | R_{avg} = %5.3e | R_{tot} = %lf\n", n+1, avg_reward, tot_reward);
                tmp_avg_reward  = 0.0;
                tot_reward      = 0.0;
            }
        }// end main training loop
    }
    
    HEADER void CLASS::init( num_t value ) {
        num_episodes_tot = 0;
        q_values.resize(model.numStates()*model.numActions(),value);
    }
    
    HEADER void CLASS::init( const std::string & qvalues_file ) {
        num_episodes_tot = 0;
        q_values.resize(model.numStates()*model.numActions(),0);
        load(qvalues_file);
    }
    
    // method to reset the table and progress made
    HEADER void CLASS::reset( num_t value ) {
        num_episodes_tot = 0;
        for( auto & qval: q_values){ qval = value; }
    }
    
    // method to look up Q value in table
    // given the state and action
    HEADER num_t & CLASS::Q(state_type s, action_type a) {
        return q_values[s + model.numStates()*a];
    }
    HEADER const num_t & CLASS::Q(state_type s, action_type a) const {
        return q_values[s + model.numStates()*a];
    }
    
    // method to look up optimal action
    // given the current state
    HEADER typename CLASS::action_type CLASS::argmaxQ( state_type s) const {
        
        // init vars for finding best action
        action_type best_action = 0;
        num_t       best_qval   = -1e10;
        num_t       tmp_qval    = 0.0;
        
        // find a^* = argmax_{a} Q(s,a)
        for( action_type a = 0; a < model.numActions(); ++a){
            tmp_qval = Q(s,a);
            if( tmp_qval > best_qval ){
                best_action = a;
                best_qval = tmp_qval;
            }
        }// end for
        
        // return the best action, a^*
        return best_action;
    }
    
    HEADER num_t CLASS::maxQ( state_type s) const {
        // init vars for finding best action
        num_t       best_qval   = -1e10;
        num_t       tmp_qval    = 0.0;
        
        // find a^* = argmax_{a} Q(s,a)
        for( action_type a = 0; a < model.numActions(); ++a){
            tmp_qval = Q(s,a);
            if( tmp_qval > best_qval ){
                best_qval = tmp_qval;
            }
        }// end for
        
        // return the best action, a^*
        return best_qval;
    }
    
    // set the discount factor
    HEADER void CLASS::setDiscountFactor( num_t dfactor ) {
        gamma = dfactor;
    }
    HEADER num_t CLASS::getDiscountFactor() const {
        return gamma;
    }
    
    HEADER void CLASS::setExplorationThreshold( num_t threshold ) {
        epsilon = threshold;
    }
    
    // methods to save and load Q values to file
    HEADER void CLASS::load( const std::string & filename ) {
        binary::vec::load_vector(filename, q_values);
    }
    HEADER void CLASS::save( const std::string & filename ) const {
        binary::vec::save_vector(filename, q_values);
    }

}// end namespace RL

// get rid of macro definitions
#undef HEADER
#undef CLASS

#endif /* q_learning_impl_hxx */



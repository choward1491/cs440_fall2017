/*
 *  q_learning.hpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/7/17
 *  Purpose:
 *
 */

#ifndef q_learning_interface_hpp
#define q_learning_interface_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <random>

namespace RL {
    
    template<typename mdp_t, typename num_t = double>
    class q_learning {
    public:
        // useful typedefs
        typedef num_t                           num_type;
        typedef mdp_t                           mdp_type;
        typedef typename mdp_t::state_type      state_type;
        typedef typename mdp_t::action_type     action_type;
        
        // ctor/dtor
        q_learning();
        ~q_learning() = default;
        
        // getter for the model
        mdp_t & getMDP();
        const mdp_t & getMDP() const;
        
        // main learning algorithm
        template<typename alpha_func, typename callback>
        void train(num_t alpha_value, unsigned int num_episodes, unsigned int count_print_msg = 1000000, callback* cback = nullptr);
        
        // method to setup the Q Learning internals
        // based on the current setup for the MDP
        void init( num_t value = 0 );
        void init( const std::string & qvalues_file );
        
        // method to reset the table and progress made
        void reset( num_t value = 0 );
        
        // method to look up Q value in table
        // given the state and action
        num_t & Q(state_type s, action_type a);
        const num_t & Q(state_type s, action_type a) const;
        
        // method to find the largest Q given some state
        num_t maxQ( state_type s) const;
        
        // method to look up optimal action
        // given the current state
        action_type argmaxQ( state_type s) const;
        
        // set the discount factor
        void setDiscountFactor( num_t dfactor );
        num_t getDiscountFactor() const;
        
        // set probability threshold that must be exceeded
        // for the action to be chosen by Q instead of randomly
        void setExplorationThreshold( num_t threshold );
        
        // methods to save and load Q values to file
        void load( const std::string & filename );
        void save( const std::string & filename ) const;
        
    private:
        mdp_t                                       model;
        unsigned int                                num_episodes_tot;
        num_t                                       gamma, epsilon;
        std::vector<num_t>                          q_values;
        std::default_random_engine                  sampler;
        
    };
    
}


#include "q_learning.hxx"

#endif /* q_learning_interface_hpp */

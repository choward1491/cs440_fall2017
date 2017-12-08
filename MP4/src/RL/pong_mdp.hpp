/*
 *  pong_mdp.hpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/7/17
 *  Purpose:
 *
 */

#ifndef pong_mdp_interface_hpp
#define pong_mdp_interface_hpp

#include <random>

namespace pong {
    namespace RL {

        // define the possible paddle actions
        enum actions: size_t {
            Nothing = 0,
            PaddleUp,
            PaddleDown
        };
        
        // define the state variable types
        enum state_vars: size_t {
            Ball_x = 0,
            Ball_y,
            BallSpeed_x,
            BallSpeed_y,
            FriendlyPaddle_y,
            OpponentPaddle_y
        };
        
        
        template<
                    typename num_t      = double,
                    typename state_t    = size_t,
                    typename action_t   = size_t
                >
        class mdp {
        public:
            
            // useful typedefs
            typedef state_t     state_type;
            typedef action_t    action_type;
            
            // useful static vars
            static const state_type             FriendlyPassedPaddleState;
            static const state_type             OpponentPassedPaddleState;
            
            //ctor/dtor
            mdp();
            ~mdp() = default;
            
            
            // set discretization vector for ith variable
            void setVariableValues(int vidx, const std::vector<num_t> & values );
            void setVariableValues(int vidx, num_t start, num_t end, int num_values );
            std::vector< std::vector<num_t> > & getVariableValues();
            const std::vector< std::vector<num_t> > & getVariableValues() const;
            
            
            // set environment domain traits
            void setEnvironmentDimensions( num_t width, num_t height);
            void setPaddleHeight( num_t pheight );
            void setTimeStep( num_t dt );
            
            // get environment domain traits
            num_t getWidth() const;
            num_t getHeight() const;
            num_t getPaddleHeight() const;
            num_t getTimeStep() const;
            
            /*
             Methods that will be used inside the Q Learning class
             */
            // method to initialize the MDP if necessary before an episode begins
            void init();
            
            // method that returns if a pong episode is complete
            bool episodeComplete() const;
            
            // method to get the discretized state hash
            state_t getState() const;
            const std::vector<num_t> & getContinuousState() const;
            
            // method to update the state of pong
            void transition( action_t a);
            
            // method to get the latest reward after
            // the last call to the `transition` method
            num_t getLatestReward() const;
            
            // get the total number of possible states
            size_t numStates() const;
            
            // get number of actions available
            size_t numActions() const;
            
            
        private:
            
            // internal state
            bool                                isSingleOpponent;
            size_t                              num_tot_states;
            size_t                              num_max_tot_vars;
            size_t                              num_tot_actions;
            num_t                               reward;
            num_t                               width, height, paddle_height;
            num_t                               time_step;
            num_t                               delta_step[3];
            state_type                          disc_state;
            std::vector< num_t >                cont_state;
            std::vector< std::vector<num_t> >   var_values;
            std::default_random_engine          sampler;
            
            
            // helper functions
            state_type componentHasher(int vidx) const;
            void updateTotalNumStates();
            void updateDiscreteStateHash();
            static num_t sign(num_t v);
            
        };

    } // end namespae RL
} // end namespace pong

#include "pong_mdp.hxx"

#endif /* pong_mdp_interface_hpp */

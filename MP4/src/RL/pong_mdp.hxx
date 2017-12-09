/*
 *  pong_mdp.hxx
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/7/17
 *  Purpose:
 *
 */

#ifndef pong_mdp_impl_hxx
#define pong_mdp_impl_hxx

// useful macros for defining class methods
#define HEADER template<typename num_t, typename state_t, typename action_t>
#define CLASS mdp<num_t, state_t, action_t>

// define useful standard lib stuff
#include <cmath>

// template class interface
#include "pong_mdp.hpp"

// other useful custom headers
#include "../utility/binary_search.hpp"

// implementation details
namespace pong {
    namespace RL {
        
        // define the possible paddle actions
        /*
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
         */

        
            // useful typedefs
            //typedef state_t     state_type;
            //typedef action_t    action_type;
        
            // internal state
            /*bool                                episodeIsComplete;
             size_t                              num_tot_vars;
             size_t                              num_tot_states;
             size_t                              num_tot_actions;
             num_t                               reward;
             state_type                          disc_state;
             std::vector< num_t >                cont_state;
             std::vector< std::vector<num_t> >   var_values;
             */
        
        // define value for special state
        HEADER const typename CLASS::state_type CLASS::FriendlyPassedPaddleState = 0;
        HEADER const typename CLASS::state_type CLASS::OpponentPassedPaddleState = 1;
        
        HEADER num_t CLASS::sign(num_t v) {
            if( v >= 0.0 ){ return 1.0; }
            return -1.0;
        }
        
        // define constructor
        HEADER CLASS::mdp():num_max_tot_vars(6),num_tot_actions(3),
                            reward(0),disc_state(0),cont_state(),var_values(),
                            isSingleOpponent(true), width(1.0), height(1.0),
                            paddle_height(0.2),time_step(1.0)
        {
            delta_step[Nothing]     = 0.0;
            delta_step[PaddleUp]    = 0.04;
            delta_step[PaddleDown]  = -0.04;
        }
        
        
        // set discretization vector for ith variable
        HEADER void CLASS::setVariableValues(int vidx, const std::vector<num_t> & values ) {
            if( var_values.size() < (vidx+1) ){ var_values.resize(vidx+1); cont_state.resize(vidx+1); }
            var_values[vidx] = values;
            updateTotalNumStates();
            isSingleOpponent = (var_values.size() < 6);
        }
        HEADER void CLASS::setVariableValues(int vidx, num_t start, num_t end, int num_values ) {
            if( var_values.size() < (vidx+1) ){ var_values.resize(vidx+1); cont_state.resize(vidx+1); }
            var_values[vidx].resize(num_values);
            num_t dv = (end - start)/static_cast<num_t>(num_values - 1);
            for(int i = 0; i < num_values; ++i){ var_values[vidx][i] = start + i*dv; }
            updateTotalNumStates();
            isSingleOpponent = (var_values.size() < 6);
        }
        HEADER std::vector< std::vector<num_t> > & CLASS::getVariableValues() {
            return var_values;
        }
        HEADER const std::vector< std::vector<num_t> > & CLASS::getVariableValues() const {
            return var_values;
        }

        // set environment domain traits
        HEADER void CLASS::setEnvironmentDimensions( num_t w, num_t h) {
            width = w; height = h;
        }
        HEADER void CLASS::setPaddleHeight( num_t pheight ) {
            paddle_height = pheight;
        }
        HEADER void CLASS::setTimeStep( num_t dt ) {
            time_step = dt;
        }
        
        // get environment domain traits
        HEADER num_t CLASS::getWidth() const {
            return width;
        }
        HEADER num_t CLASS::getHeight() const {
            return height;
        }
        HEADER num_t CLASS::getPaddleHeight() const {
            return paddle_height;
        }
        HEADER num_t CLASS::getTimeStep() const {
            return time_step;
        }
        
        /*
         Methods that will be used inside the Q Learning class
         */
        
        // method to initialize the MDP if necessary before an episode begins
        HEADER void CLASS::init() {
            
            // init continuous state representation
            cont_state[Ball_x]              = width*0.5;
            cont_state[Ball_y]              = height*0.5;
            cont_state[BallSpeed_x]         = 0.03;
            cont_state[BallSpeed_y]         = 0.01;
            cont_state[FriendlyPaddle_y]    = 0.5 * (height - paddle_height);
            if( !isSingleOpponent ){
                cont_state[OpponentPaddle_y]    = 0.5 * (height - paddle_height);
            }
            
            // update the discrete state representation
            updateDiscreteStateHash();
        }
        
        // method that returns if a pong episode is complete
        HEADER bool CLASS::episodeComplete() const {
            return (disc_state == FriendlyPassedPaddleState) || (disc_state == OpponentPassedPaddleState);
        }

        // method to get the discretized state hash
        HEADER state_t CLASS::getState() const {
            return disc_state;
        }
        HEADER const std::vector<num_t> & CLASS::getContinuousState() const {
            return cont_state;
        }

        // method to update the state of pong
        HEADER void CLASS::transition( action_t a ) {
            
            // define uniform distribution for later use
            std::uniform_real_distribution<num_t> U(-1,1);
            
            // set friendly and opponent paddle x positions
            num_t fpaddle_x = width, opaddle_x = 0.0;
            
            // update friendly paddle position
            cont_state[FriendlyPaddle_y] += delta_step[a];
            if( cont_state[FriendlyPaddle_y] < 0 )          { cont_state[FriendlyPaddle_y] = 0; }
            else if( cont_state[FriendlyPaddle_y] > (height - paddle_height) ){ cont_state[FriendlyPaddle_y] = (height - paddle_height); }
            num_t fpaddle_corners[2] = { cont_state[FriendlyPaddle_y] + paddle_height,
                                        cont_state[FriendlyPaddle_y]};
            num_t opaddle_corners[2] = { 0 };
            
            // update ball position
            num_t prevBall_x    = cont_state[Ball_x];
            num_t prevBall_y    = cont_state[Ball_y];
            cont_state[Ball_x] += time_step*cont_state[BallSpeed_x];
            cont_state[Ball_y] += time_step*cont_state[BallSpeed_y];
            num_t fy_intersect  = prevBall_y + (width - prevBall_x)*(cont_state[RL::Ball_y]-prevBall_y)/(cont_state[RL::Ball_x]-prevBall_x);
            
            // update opponent paddle if necessary
            if( !isSingleOpponent ){
                
                // difference between vertical ball pos and opponent paddle vertical pos
                num_t dy = cont_state[Ball_y] - (cont_state[OpponentPaddle_y] + 0.5*paddle_height);
                
                // update paddle position
                if( dy > 0 ){
                    cont_state[OpponentPaddle_y] += 0.5*delta_step[PaddleUp];
                }else if ( dy < 0 ){
                    cont_state[OpponentPaddle_y] += 0.5*delta_step[PaddleDown];
                }
                
                // bound the opponent's paddle pos
                if( cont_state[OpponentPaddle_y] < 0 )          { cont_state[OpponentPaddle_y] = 0; }
                else if( cont_state[OpponentPaddle_y] > (height-paddle_height) ){ cont_state[OpponentPaddle_y] = (height-paddle_height); }
                
                // set corner positions for the opponent paddle
                opaddle_corners[0] = cont_state[OpponentPaddle_y] + paddle_height;
                opaddle_corners[1] = cont_state[OpponentPaddle_y];
            }
            
            // check for collision with paddle(s) and walls
            // and make sure to handle this dynamics change
            bool bouncedOffPaddle = false;
            
            // check for bounce off of top of environment (y direction is down)
            if( cont_state[Ball_y] < 0.0 ){
                cont_state[Ball_y]      = -cont_state[Ball_y];
                cont_state[BallSpeed_y] = -cont_state[BallSpeed_y];
            }
            // check for bounce off of the bottom of environment
            else if( cont_state[Ball_y] > height ){
                cont_state[Ball_y]      = 2.0*height - cont_state[Ball_y];
                cont_state[BallSpeed_y] = -cont_state[BallSpeed_y];
            }
            // check for bounce off of the left of the environment
            if( cont_state[Ball_x] < 0.0 ){
                
                num_t y_intersect = prevBall_y + (0.0 - prevBall_x)*(cont_state[RL::Ball_y]-prevBall_y)/(cont_state[RL::Ball_x]-prevBall_x);
                if( isSingleOpponent ){ // if this should be a wall
                    cont_state[Ball_x]      = -cont_state[Ball_x];
                    cont_state[BallSpeed_x] = -cont_state[BallSpeed_x];
                    
                }else if( (cont_state[Ball_x] <= opaddle_x && opaddle_x <= prevBall_x) &&
                        (opaddle_corners[0] >= y_intersect && opaddle_corners[1] <= y_intersect ))
                {
                    // update ball state
                    cont_state[Ball_x]      = -cont_state[Ball_x];
                    cont_state[BallSpeed_x] = -cont_state[BallSpeed_x]  + 0.015*U(sampler);
                    cont_state[BallSpeed_y] = cont_state[BallSpeed_y]   + 0.03*U(sampler);
                }
            }
            // check for bounce off of friendly paddle
            else if( (cont_state[Ball_x] >= fpaddle_x && fpaddle_x >= prevBall_x) &&
                     (fpaddle_corners[0] >= fy_intersect && fpaddle_corners[1] <= fy_intersect )
                    )
            {
                // set flag as true that ball bounced off of paddle
                bouncedOffPaddle = true;
                
                // update ball state
                cont_state[Ball_x]      = 2*fpaddle_x - cont_state[Ball_x];
                cont_state[BallSpeed_x] = -cont_state[BallSpeed_x]  + 0.015*U(sampler);
                cont_state[BallSpeed_y] = cont_state[BallSpeed_y]   + 0.03*U(sampler);
            }
            
            // bound velocities if necessary
            if( std::abs(cont_state[BallSpeed_x]) < 0.03 ){
                cont_state[BallSpeed_x] = 0.03 * sign(cont_state[BallSpeed_x]);
            }
            if( std::abs(cont_state[BallSpeed_x]) > 1.0 ){
                cont_state[BallSpeed_x] = sign(cont_state[BallSpeed_x]);
            }
            if( std::abs(cont_state[BallSpeed_y]) > 1.0 ){
                cont_state[BallSpeed_y] = sign(cont_state[BallSpeed_y]);
            }
            
            // update the discrete state
            updateDiscreteStateHash();
            
            // do reward related checks
            if( cont_state[Ball_x] > width ){
                reward = -1.0;
                disc_state = FriendlyPassedPaddleState;
            }
            else if( cont_state[Ball_x] < 0 ){
                reward = 10;
                disc_state = OpponentPassedPaddleState;
            }
            else if( bouncedOffPaddle ) { reward = 1.0; }
            else                        { reward = 0.0; }
            
        }

        // method to get the latest reward after
        // the last call to the `transition` method
        HEADER num_t CLASS::getLatestReward() const {
            return reward;
        }

        // get the total number of possible states
        HEADER size_t CLASS::numStates() const {
            return num_tot_states;
        }

        // get number of actions available
        HEADER size_t CLASS::numActions() const {
            return num_tot_actions;
        }

        // helper functions
        HEADER void CLASS::updateDiscreteStateHash() {
            state_type hash = 2;
            state_type factor = 1;
            for(size_t idx = 0; idx < cont_state.size(); idx++){
                hash += factor*componentHasher(idx);
                factor *= var_values[idx].size();
            }
            disc_state = hash;
        }
        
        HEADER void CLASS::updateTotalNumStates() {
            num_tot_states = 1;
            for(size_t idx = 0; idx < var_values.size(); idx++){
                num_tot_states *= var_values[idx].size();
            }
            ++num_tot_states; // add one for friendly special state
            ++num_tot_states; // add one for opponent special state
        }
        
        HEADER typename CLASS::state_type CLASS::componentHasher(int vidx) const {
            switch(vidx){
                case Ball_x:        return algos::binary_search(cont_state[vidx], &var_values[vidx][0], var_values[vidx].size());
                case Ball_y:        return algos::binary_search(cont_state[vidx], &var_values[vidx][0], var_values[vidx].size());
                case BallSpeed_x:   return (cont_state[vidx] > 0);
                case BallSpeed_y: {
                    if( std::abs(cont_state[vidx]) < 0.015 ){ return 1; }
                    return (cont_state[vidx] > 0) ? 2 : 0;
                }
                case FriendlyPaddle_y: {
                    if( cont_state[vidx] >= (height - paddle_height) ){ return 11; }
                    return (12 * cont_state[vidx] / (height - paddle_height));
                }
                case OpponentPaddle_y: {
                    if( cont_state[vidx] >= (height - paddle_height) ){ return 11; }
                    return (12 * cont_state[vidx] / (height - paddle_height));
                }
                default:                return 0;
            }
        }
        

    }// end namespace RL
}// end namespace pong

// get rid of macro definitions
#undef HEADER
#undef CLASS

#endif /* pong_mdp_impl_hxx */



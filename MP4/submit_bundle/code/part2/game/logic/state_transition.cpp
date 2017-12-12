/*
 *  state_transition.cpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#include <random>
#include <cmath>
#include "state_transition.hpp"
#include "state_hasher.hpp"
#include "../../RL/pong_mdp.hpp"

namespace pong {
    
    double sign(double v) {
        if( v >= 0.0 ){ return 1.0; }
        return -1.0;
    }
    
    size_t state_transition( std::vector<double> & state,
                            double newPlayer1PaddlePos, double newPlayer2PaddlePos,
                            bool player1IsWall, bool player2IsWall)
    {
        // define some useful typdefs
        typedef double      num_t;
        typedef RL::mdp<>   mdp_t;
        
        // define some static variables for the state transition logic
        static std::default_random_engine sampler(17);
        static const double height = 1.0, width = 1.0, paddle_height = 0.2;
        static const double time_step = 1.0, delta_step[3] = {0, 0.04, -0.04};
        
        // define uniform distribution for later use
        std::uniform_real_distribution<num_t> U(-1,1);
        
        // set friendly and opponent paddle x positions
        num_t fpaddle_x = width, opaddle_x = 0.0;
        
        // initialize corner positions
        num_t fpaddle_corners[2] = { 0 };
        num_t opaddle_corners[2] = { 0 };
        
        // update ball position
        num_t prevBall_x    = state[RL::Ball_x];
        num_t prevBall_y    = state[RL::Ball_y];
        state[RL::Ball_x] += time_step*state[RL::BallSpeed_x];
        state[RL::Ball_y] += time_step*state[RL::BallSpeed_y];
        //sampler.seed(17);
        
        // update friendly paddle if necessary
        if( !player1IsWall ){
            
            // update player two
            state[RL::FriendlyPaddle_y] = std::max(0.0, std::min(newPlayer1PaddlePos,height - paddle_height));
            
            // set corner positions for the opponent paddle
            fpaddle_corners[0] = state[RL::FriendlyPaddle_y] + paddle_height;
            fpaddle_corners[1] = state[RL::FriendlyPaddle_y];
        }
        
        // update opponent paddle if necessary
        if( !player2IsWall ){
            
            // update player two
            state[RL::OpponentPaddle_y] = std::max(0.0, std::min(newPlayer2PaddlePos,height - paddle_height));
            
            // set corner positions for the opponent paddle
            opaddle_corners[0] = state[RL::OpponentPaddle_y] + paddle_height;
            opaddle_corners[1] = state[RL::OpponentPaddle_y];
        }
        
        // check for collision with paddle(s) and walls
        // and make sure to handle this dynamics change
        bool bouncedOffPaddle = false;
        
        // check for bounce off of top of environment (y direction is down)
        if( state[RL::Ball_y] < 0.0 ){
            state[RL::Ball_y]      = -state[RL::Ball_y];
            state[RL::BallSpeed_y] = -state[RL::BallSpeed_y];
        }
        // check for bounce off of the bottom of environment
        else if( state[RL::Ball_y] > height ){
            state[RL::Ball_y]      = 2.0*height - state[RL::Ball_y];
            state[RL::BallSpeed_y] = -state[RL::BallSpeed_y];
        }
        // check for bounce off of the left of the environment
        if( state[RL::Ball_x] < 0.0 ){
            double y_intersect = prevBall_y + (0.0 - prevBall_x)*(state[RL::Ball_y]-prevBall_y)/(state[RL::Ball_x]-prevBall_x);
            if( player2IsWall ){ // if this should be a wall
                state[RL::Ball_x]      = -state[RL::Ball_x];
                state[RL::BallSpeed_x] = -state[RL::BallSpeed_x]  + 0.0*0.015*U(sampler);
                //state[RL::BallSpeed_y] = state[RL::BallSpeed_y]   + 0.03*U(sampler);
                
            }else if( (state[RL::Ball_x] <= opaddle_x && opaddle_x <= prevBall_x) &&
                     (opaddle_corners[0] >= y_intersect && opaddle_corners[1] <= y_intersect ))
            {
                // update ball state
                state[RL::Ball_x]      = -state[RL::Ball_x];
                state[RL::BallSpeed_x] = -state[RL::BallSpeed_x] + 0.015*U(sampler);
                state[RL::BallSpeed_y] = state[RL::BallSpeed_y] + 0.03*U(sampler);
            }
        }
        // check for bounce off right of environment
        if( state[RL::Ball_x] > width ){
            
            double y_intersect = prevBall_y + (width - prevBall_x)*(state[RL::Ball_y]-prevBall_y)/(state[RL::Ball_x]-prevBall_x);
            if( player1IsWall ){ // if this should be a wall
                state[RL::Ball_x]      = 2*width - state[RL::Ball_x];
                state[RL::BallSpeed_x] = -state[RL::BallSpeed_x]  + 0.0*0.015*U(sampler);
                //state[RL::BallSpeed_y] = state[RL::BallSpeed_y]   + 0.03*U(sampler);
                
            }else if( (state[RL::Ball_x] >= fpaddle_x && fpaddle_x >= prevBall_x) &&
                     (fpaddle_corners[0] >= y_intersect && fpaddle_corners[1] <= y_intersect ))
            {
                // set flag as true that ball bounced off of paddle
                bouncedOffPaddle = true;
                
                // update ball state
                state[RL::Ball_x]      = 2*fpaddle_x - state[RL::Ball_x];
                state[RL::BallSpeed_x] = -state[RL::BallSpeed_x] + 0.015*U(sampler);
                state[RL::BallSpeed_y] = state[RL::BallSpeed_y] + 0.03*U(sampler);
            }
        }
        
        // bound velocities if necessary
        if( std::abs(state[RL::BallSpeed_x]) < 0.03 ){
            state[RL::BallSpeed_x] = 0.03 * sign(state[RL::BallSpeed_x]);
        }
        if( std::abs(state[RL::BallSpeed_x]) > 1.0 ){
            state[RL::BallSpeed_x] = sign(state[RL::BallSpeed_x]);
        }
        if( std::abs(state[RL::BallSpeed_y]) > 1.0 ){
            state[RL::BallSpeed_y] = sign(state[RL::BallSpeed_y]);
        }
        
        // update the discrete state
        size_t disc_state = 17; // initialize to value outside of final states
        
        // do checks for final state
        if( state[RL::Ball_x] > width ){
            disc_state = mdp_t::FriendlyPassedPaddleState;
        }
        else if( state[RL::Ball_x] < 0 ){
            disc_state = mdp_t::OpponentPassedPaddleState;
        }
        
        // return the discrete state
        return disc_state;
    }
}

//
// Created by Christian J Howard on 10/29/17.
//

#ifndef SRC_BREAKTHROUGH_COSTFUNC_HPP_HPP
#define SRC_BREAKTHROUGH_COSTFUNC_HPP_HPP

// standard library stuff
#include <vector>

// game playing agent stuff
#include "breakthrough_rules.hpp"
#include "breakthrough_extended_rules.hpp"
#include "breakthrough_provided_heuristics.hpp"
#include "game_instance.hpp"
#include "breakthrough_minimax_agent.hpp"
#include "breakthrough_alphabeta_agent.hpp"
#include "breakthrough_utility_ab.hpp"
#include "breakthrough_learned_heuristics.hpp"

#define NR 8
#define NC 8

// game typedefs
typedef bt::baseline_rules<NR,NC> bt_rules;
typedef game::agent<bt_rules>     bt_agent;
typedef bt::minimax<bt_rules>     bt_minimax;
typedef bt::alphabeta<bt_rules>   bt_ab;
typedef bt::utility_ab<bt_rules>  bt_uab;
typedef game::instance<bt_rules>  bt_game;

namespace bt {

    template<typename num_t>
    class costfunc {
    public:
        static uint32_t numDims() {
            return 7;
        }

        num_t operator()( const std::vector<num_t> & x ){
            bt::provided::defensive<NR,NC>  defensive_h;
            bt::provided::offensive<NR,NC>  offensive_h;
            bt::learned::defensive<NR,NC>   dlearn_h;
            bt::learned::offensive<NR,NC>   olearn_h;

            uint32_t numMC = 5;
            bt_game game;
            bt_ab p1, p2;

            // init variable to track how many times
            // the learned heuristic wins against baseline
            int totalWon = 0;

            // initialize player 1 (baseline)
            // first optimization will be using defensive heuristic
            p1.setMaxSearchDepth(2);
            p1.setUtilityEstimator(defensive_h);

            // initialize player 2 (learned)
            // first optimization will be using offensive heuristic
            p2.setMaxSearchDepth(2);
            p2.setUtilityEstimator(olearn_h);
            for(int i =0; i < numDims(); ++i) {
                olearn_h.setCoefficient(i, x[i]);
                dlearn_h.setCoefficient(i, x[i]);
            }

            // add players to the game
            game.addPlayer1(&p1);
            game.addPlayer2(&p2);

            // run monte carlo simulation of game play and keep track
            // of how many times player2 (learned heuristic) wins so
            // we can estimate a probability of winning
            for(uint32_t n = 0; n < numMC; ++n) {
                game.play();
                totalWon += game.getPlayerWhoWon();
            }

            // return the negative of the probability of the learned heuristic
            // winning against the baseline heuristic
            return -100.0*static_cast<num_t>(totalWon)/static_cast<num_t>(numMC);
        }


    };

}

#endif //SRC_BREAKTHROUGH_COSTFUNC_HPP_HPP

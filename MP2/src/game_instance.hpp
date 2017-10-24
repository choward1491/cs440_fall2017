//
// Created by Christian J Howard on 10/24/17.
//

#ifndef SRC_GAME_INSTANCE_HPP
#define SRC_GAME_INSTANCE_HPP

#include "game_agent.hpp"

#ifdef DEBUG
#define DEBUG_PRINT() gameState.print()
#else
#define DEBUG_PRINT()
#endif

namespace game {

    template<typename Rules, typename num_val = double>
    class instance {
    public:
        typedef agent<Rules,num_val>             agent_t;
        typedef typename Rules::state_t          state_t;
        typedef typename Rules::action_t         action_t;
        typedef typename Rules::transition_t     transition_t;

        void addPlayer1( agent_t* agent) { agent->setTeam(0); p1 = agent; }
        void addPlayer2( agent_t* agent) { agent->setTeam(1); p2 = agent; }
        void play() {
            enum team_type { team1 = 0, team2, none};
            gameState.init();
            gameState.print();
            agent_t* agentp = nullptr;
            int turn = 0;
            size_t totalTurns = 0;
            while(!rules.isGameComplete( gameState )){
                switch(turn){
                    case team1: agentp = p1;break;
                    case team2: agentp = p2;break;
                }
                turn = (turn+1) % 2;
                gameState = F(gameState,agentp->getNextMove(gameState));
                gameState.print();
            }
        }
        state_t getFinalGameState() const { return gameState; }

    private:
        state_t gameState;
        Rules rules;
        transition_t F;
        agent_t *p1, *p2;

    };

}


#endif //SRC_GAME_INSTANCE_HPP

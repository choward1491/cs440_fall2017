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
            p1->initPlayer();
            p2->initPlayer();
            //gameState.print();
            agent_t* agentp = nullptr;
            uint32_t* counter_p = nullptr;
            int turn = 0;
            size_t totalTurns = 0;
            numP1Moves = numP2Moves = 0;
            while(!rules.isGameComplete( gameState )){
                switch(turn){
                    case team1: agentp = p1; counter_p = &numP1Moves; break;
                    case team2: agentp = p2; counter_p = &numP2Moves; break;
                }
                turn = (turn+1) % 2;
                gameState = F(gameState,agentp->getNextMove(gameState));
                (*counter_p)++;
                //gameState.print();
            }
            playerWhoWon = (turn+1) % 2;
        }
        state_t getFinalGameState() const { return gameState; }
        uint32_t getNumMoves(int player_number) const {
            switch(player_number){
                case 0:     return numP1Moves;
                case 1:
                default:    return numP2Moves;
            }
        }

        uint32_t getNumPiecesCapturedBy(int player_number) const {
            return gameState.getNumPiecesCapturedBy(player_number);
        }

        double getAvgMoveTimeFor( int player_number ) const {
            switch(player_number){
                case 0:     return p1->getAvgMoveTimeInMilliseconds();
                case 1:
                default:    return p2->getAvgMoveTimeInMilliseconds();
            }
        }

        double getAvgNumberNodesExpandedPerMoveFor( int player_number ) const {
            switch(player_number){
                case 0:     return p1->getAvgExpandedNodes();
                case 1:
                default:    return p2->getAvgExpandedNodes();
            }
        }

        uint32_t getTotalNodesExpandedFor( int player_number ) const {
            switch(player_number){
                case 0:     return p1->getNumNodesExpanded();
                case 1:
                default:    return p2->getNumNodesExpanded();
            }
        }

        int getPlayerWhoWon() const {
            return playerWhoWon;
        }

    private:
        bool printStateProgress;
        int playerWhoWon;
        state_t gameState;
        Rules rules;
        transition_t F;
        agent_t *p1, *p2;
        uint32_t numP1Moves;
        uint32_t numP2Moves;

    };

}


#endif //SRC_GAME_INSTANCE_HPP

/*
 *  pong_game.hpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */

#ifndef pong_game_hpp
#define pong_game_hpp

#include <vector>
#include <memory>
#include "pong_agent.hpp"
#include "pong_mdp.hpp"
#include "../../utility/config_parser.hpp"
#include "../gui/pong_gui.hpp"

namespace pong {
    
    class game {
    public:
        
        // ctor/dtor
        game(gui* pong_gui, int frames_per_second=15);
        ~game() = default;
        
        // game play related info
        bool play( enum agent_type p1_type, enum agent_type p2_type );
        void reset();
        
        // getters
        unsigned int getNumberOfWins(int idx);
        const std::vector<double> & getState() const;
        enum agent_type getPlayerType(int idx) const;
        
        // setters
        void setConfigFileContents( const parser::config* cparser );
        
    private:
        
        // internal game state
        bool isSinglePlayer;
        unsigned int playerWins[2], num_games;
        size_t state_hash;
        RL::mdp<>   mdp[2];
        std::vector<double> state;
        std::vector<std::unique_ptr<agent>> players;
        const parser::config* configp;
        const double FPS;
        
        // allegro items
        ALLEGRO_EVENT_QUEUE *event_queue;
        ALLEGRO_TIMER       *timer;
        gui                 *pong_gui;
        
        // method to initialize the game
        void init();
        void swap();
        
        // methods for playing a game
        void waitForHumanReadyIfPlaying(bool & stopPlaying, bool & gameComplete);
        bool playIndividualGUIGame();
        bool playIndividualGameWithNoGUI();
        
    };
    
}// end namespace pong

#endif /* pong_game_hpp */

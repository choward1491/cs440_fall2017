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
#include "pong_agent.hpp"
#include "pong_mdp.hpp"
#include "../../utility/config_parser.hpp"
#include "../gui/pong_gui.hpp"

namespace pong {
    
    class game {
    public:
        
        // ctor/dtor
        game(gui* pong_gui);
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
        unsigned int playerWins[2];
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
    };
    
}// end namespace pong

#endif /* pong_game_hpp */

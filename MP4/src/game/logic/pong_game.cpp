/*
 *  pong_game.cpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/8/17
 *  Purpose:
 *
 */


// standard lib includes
#include <chrono>
#include <allegro5/allegro.h>

// high level pong related includes
#include "pong_game.hpp"
#include "state_hasher.hpp"
#include "../../RL/pong_mdp.hpp"
#include "state_transition.hpp"

// include the agents
#include "wall_agent.hpp"
#include "predefined_agent.hpp"
#include "q_agent.hpp"
#include "human_agent.hpp"

// include useful exception handling
#include "../../utility/custom_exception.hpp"

namespace pong {
    
    /*
     unsigned int playerWins[2];
     std::vector<double> state;
     std::vector<std::unique_ptr<agent>> players;
     */
    enum player: int {
        Player1 = 0,
        Player2
    };
    
    enum game_type: int {
        Single = 0,
        Two
    };
    
    enum human_keys: int {
        P1_UP = 0, P1_DWN, P2_UP, P2_DWN
    };
    
    // ctor/dtor
    game::game(gui* pgui):isSinglePlayer(false),players(2),
        configp(nullptr),FPS(15.0),event_queue(nullptr),timer(nullptr),pong_gui(pgui)
    {
        playerWins[Player1] = 0;
        playerWins[Player2] = 0;
        
        mdp[Single].setVariableValues(pong::RL::Ball_x, 0.0, 1.0, 12);
        mdp[Single].setVariableValues(pong::RL::Ball_y, 0.0, 1.0, 12);
        mdp[Single].setVariableValues(pong::RL::BallSpeed_x, {-1, 1});
        mdp[Single].setVariableValues(pong::RL::BallSpeed_y, {-1, 0, 1});
        mdp[Single].setVariableValues(pong::RL::FriendlyPaddle_y, 0.0, 1.0, 12);
        
        mdp[Two].setVariableValues(pong::RL::Ball_x, 0.0, 1.0, 12);
        mdp[Two].setVariableValues(pong::RL::Ball_y, 0.0, 1.0, 12);
        mdp[Two].setVariableValues(pong::RL::BallSpeed_x, {-1, 1});
        mdp[Two].setVariableValues(pong::RL::BallSpeed_y, {-1, 0, 1});
        mdp[Two].setVariableValues(pong::RL::FriendlyPaddle_y, 0.0, 1.0, 12);
        mdp[Two].setVariableValues(pong::RL::OpponentPaddle_y, 0.0, 1.0, 12);
        
        // initialize keyboard input source
        if(!al_install_keyboard()) { throw custom::exception("failed to initialize the keyboard!"); }
    }
    
    void game::setConfigFileContents( const parser::config* cparser ) {
        configp = cparser;
    }
    
    void game::init() {
        state[RL::Ball_x]              = width*0.5;
        state[RL::Ball_y]              = height*0.5;
        state[RL::BallSpeed_x]         = 0.03;
        state[RL::BallSpeed_y]         = 0.01;
        state[RL::FriendlyPaddle_y]    = 0.5 * (height - paddle_height);
        if( !isSinglePlayer ){
            state[RL::OpponentPaddle_y]    = 0.5 * (height - paddle_height);
        }
    }
    void game::swap() {
        double tmp                  = state[RL::FriendlyPaddle_y];
        state[RL::FriendlyPaddle_y] = state[RL::OpponentPaddle_y];
        state[RL::OpponentPaddle_y] = tmp;
    }
    bool game::play( enum agent_type p1_type, enum agent_type p2_type ) {
        
        // figure out if the game is single player
        if( p1_type == Wall || p2_type == Wall ){ isSinglePlayer = true; }
        
        // resize the state based on if it is single player or not
        if( isSinglePlayer ){ state.resize(5,0); }
        else                { state.resize(6,0); }
        
        // initialize the game state
        init();
        
        // allocate the agents
        enum agent_type ptypes[2] = {p1_type, p2_type};
        std::string qtable_ext[2] = {"1","2"};
        for(int i = 0; i < 2; ++i){
            switch(ptypes[i]){
                case Wall:{
                    players[i] = std::make_unique<wall>();
                }   break;
                case Predefined:{
                    players[i] = std::make_unique<predefined>();
                }   break;
                case Human:{
                    players[i] = std::make_unique<human>();
                }   break;
                case QAgent:{
                    players[i] = std::make_unique<q_agent>();
                    q_agent* qag = dynamic_cast<q_agent*>(players[i].get());
                    if( !configp ){ throw custom::exception("Config file contents not passed to pong game instance.");}
                    qag->setMDP(mdp[Two]);
                    std::string qtable_file = configp->retrieve<std::string>("qvalues"+qtable_ext[i]);
                    qag->loadController(qtable_file);
                }   break;
            }// end switch
        }//end for loop
        
        // setup event queue and related stuff
        timer = al_create_timer(1.0 / FPS);
        if(!timer) { throw custom::exception("failed to create timer!"); }
        
        event_queue = al_create_event_queue();
        if(!event_queue) {
            al_destroy_timer(timer);
            throw custom::exception("failed to create the event queue!");
        }
        
        // register sources for events
        al_register_event_source(event_queue, al_get_display_event_source(pong_gui->getDisplayRef()));
        al_register_event_source(event_queue, al_get_timer_event_source(timer));
        al_register_event_source(event_queue, al_get_keyboard_event_source());
        
        
        // play the game
        bool stopPlaying        = false;
        bool redraw             = false;
        bool gameComplete       = false;
        bool handleNonHumans    = false;
        bool key[4] = {0};
        double newPlayerPos[2] = {0};
        size_t state_hash = 17;
        
        al_start_timer(timer);
        while(!gameComplete)
        {
            ALLEGRO_EVENT ev;
            
            if( !handleNonHumans ){
                if( players[Player1]->getAgentType() != Human ){
                    newPlayerPos[Player1] = players[Player1]->updatePosition(state);
                }
                if( players[Player2]->getAgentType() != Human ){
                    swap();
                    newPlayerPos[Player2] = players[Player2]->updatePosition(state);
                    swap();
                }
                handleNonHumans = true;
            }
            
            al_wait_for_event(event_queue, &ev);
            
            if(ev.type == ALLEGRO_EVENT_TIMER) {
                if( players[Player1]->getAgentType() == Human ){
                    human* p1 = dynamic_cast<human*>(players[Player1].get());
                    if(key[P1_UP] ) {
                        p1->setKeyInput(human::Down);
                    }
                    
                    if(key[P1_DWN]) {
                        p1->setKeyInput(human::Up);
                    }
                    
                    newPlayerPos[Player1] = p1->updatePosition(state);
                }
                
                if( players[Player2]->getAgentType() == Human ){
                    human* p2 = dynamic_cast<human*>(players[Player2].get());
                    if(key[P2_UP] ) {
                        p2->setKeyInput(human::Down);
                    }
                    
                    if(key[P2_DWN]) {
                        p2->setKeyInput(human::Up);
                    }
                    swap();
                    newPlayerPos[Player2] = p2->updatePosition(state);
                    swap();
                }
                
                redraw = true;
            }
            else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                gameComplete    = true;
                stopPlaying     = true;
                break;
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch(ev.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        key[P1_UP] = true;
                        break;
                        
                    case ALLEGRO_KEY_DOWN:
                        key[P1_DWN] = true;
                        break;
                        
                    case ALLEGRO_KEY_W:
                        key[P2_UP] = true;
                        break;
                        
                    case ALLEGRO_KEY_S:
                        key[P2_DWN] = true;
                        break;
                }
            }
            else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
                switch(ev.keyboard.keycode) {
                    case ALLEGRO_KEY_UP:
                        key[P1_UP] = false;
                        break;
                        
                    case ALLEGRO_KEY_DOWN:
                        key[P1_DWN] = false;
                        break;
                        
                    case ALLEGRO_KEY_W:
                        key[P2_UP] = false;
                        break;
                        
                    case ALLEGRO_KEY_S:
                        key[P2_DWN] = false;
                        break;
                        
                    case ALLEGRO_KEY_ESCAPE:
                        stopPlaying     = true;
                        gameComplete    = true;
                        break;
                }
            }
            
            if(redraw && al_is_event_queue_empty(event_queue)) {
                
                // update useful boolean flags
                redraw          = false;
                handleNonHumans = false;
                bool playerAreWalls[2] = {  players[Player1]->getAgentType() == Wall,
                                            players[Player2]->getAgentType() == Wall };
                
                // update the state
                state_hash = state_transition(state, newPlayerPos[Player1], newPlayerPos[Player2],
                                                    playerAreWalls[Player1], playerAreWalls[Player2]);
                
                // check if game is complete
                gameComplete    = gameComplete || !(state_hash != RL::mdp<>::FriendlyPassedPaddleState
                                                    && state_hash != RL::mdp<>::OpponentPassedPaddleState);
                
                // update the GUI
                pong_gui->drawGame(state, playerWins, playerAreWalls);
            }
        }// main game loop
        
        // increment the win score count if game ended normally
        if( !stopPlaying ){
            ++playerWins[(state_hash+1)%2];
        }
        
        // clear the event queue related variables
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        
        // return flag
        return stopPlaying;
    }
    void game::reset() {
        playerWins[0] = playerWins[1] = 0;
    }
    
    // getters
    unsigned int game::getNumberOfWins(int idx) {
        return playerWins[idx];
    }
    const std::vector<double> & game::getState() const {
        return state;
    }
    enum agent_type game::getPlayerType(int idx) const {
        return players[idx]->getAgentType();
    }


    
}

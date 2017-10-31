//
// Created by Christian J Howard on 10/30/17.
//

#include <string>
#include <vector>

#include "FileWrap.hpp"
#include "breakthrough_test_games.hpp"
#include "breakthrough_rules.hpp"
#include "breakthrough_extended_rules.hpp"
#include "breakthrough_provided_heuristics.hpp"
#include "game_instance.hpp"
#include "breakthrough_minimax_agent.hpp"
#include "breakthrough_alphabeta_agent.hpp"
#include "breakthrough_utility_ab.hpp"
#include "breakthrough_cone_heuristics.hpp"
#include "breakthrough_handcraft_heuristics.hpp"
#include "breakthrough_learned_heuristics.hpp"


// rule typedefs
typedef bt::baseline_rules<8,8> bt_rules8x8;
typedef bt::baseline_rules<5,10> bt_rules5x10;
typedef bt::extended_rules<8,8> ex_rules8x8;

// typedef bt 8x8 nominal ones
typedef game::agent<bt_rules8x8>     agent8x8n;
typedef bt::minimax<bt_rules8x8>     bt_minimax8x8n;
typedef bt::utility_ab<bt_rules8x8>  bt_uab8x8n;
typedef game::instance<bt_rules8x8>  bt_game8x8n;
typedef bt::provided::defensive<8,8,bt::baseline_rules>  pd_8x8n;
typedef bt::provided::offensive<8,8,bt::baseline_rules>  po_8x8n;
typedef bt::learned::defensive<8,8,bt::baseline_rules>   ld_8x8n;
typedef bt::learned::offensive<8,8,bt::baseline_rules>   lo_8x8n;
typedef bt::handcraft::defensive<8,8,bt::baseline_rules> hd_8x8n;
typedef bt::handcraft::offensive<8,8,bt::baseline_rules> ho_8x8n;

// typedef bt 5x10 nominal ones
typedef game::agent<bt_rules5x10>     agent5x10n;
typedef bt::minimax<bt_rules5x10>     bt_minimax5x10n;
typedef bt::utility_ab<bt_rules5x10>  bt_uab5x10n;
typedef game::instance<bt_rules5x10>  bt_game5x10n;
typedef bt::provided::defensive<5,10,bt::baseline_rules>  pd_5x10n;
typedef bt::provided::offensive<5,10,bt::baseline_rules>  po_5x10n;
typedef bt::learned::defensive<5,10,bt::baseline_rules>   ld_5x10n;
typedef bt::learned::offensive<5,10,bt::baseline_rules>   lo_5x10n;
typedef bt::handcraft::defensive<5,10,bt::baseline_rules> hd_5x10n;
typedef bt::handcraft::offensive<5,10,bt::baseline_rules> ho_5x10n;

// typedef bt 8x8 extended ones
typedef game::agent<ex_rules8x8>     agent8x8e;
typedef bt::minimax<ex_rules8x8>     bt_minimax8x8e;
typedef bt::utility_ab<ex_rules8x8>  bt_uab8x8e;
typedef game::instance<ex_rules8x8>  bt_game8x8e;
typedef bt::provided::defensive<8,8,bt::extended_rules>  pd_8x8e;
typedef bt::provided::offensive<8,8,bt::extended_rules>  po_8x8e;
typedef bt::learned::defensive<8,8,bt::extended_rules>   ld_8x8e;
typedef bt::learned::offensive<8,8,bt::extended_rules>   lo_8x8e;
typedef bt::handcraft::defensive<8,8,bt::extended_rules> hd_8x8e;
typedef bt::handcraft::offensive<8,8,bt::extended_rules> ho_8x8e;

#define ch_print0(x1) fprintf(fout,x1); printf(x1)
#define ch_print1(x1,x2) fprintf(fout,x1,x2); printf(x1,x2)
#define ch_print4(x1,x2,x3,x4,x5) fprintf(fout,x1,x2,x3,x4,x5); printf(x1,x2,x3,x4,x5)


namespace bt {
    namespace test {

#define T(starter) starter##8x8n
        void matchups_8x8nominal() {

            // useful typedefs
            typedef T(pd_)::parent h_t;
            typedef T(bt_game) game_t;
            typedef game_t::state_t state_t;

            // define output file
            wrap::file fout("matchup_8x8n.txt",wrap::file::Write);

            // define agents that will be used
            T(bt_minimax)  mm;
            T(bt_uab)      uab1, uab2;

            // define heuristics
            T(pd_) dh1;
            T(po_) oh1;
            T(hd_) dh2;
            T(ho_) oh2;
            T(ld_) dh3;
            T(lo_) oh3;

            // define lists for matchup stuff
            std::vector<int> player1_sd(6,4); player1_sd[0] = 3;
            std::vector<int> player2_sd(6,4);
            std::vector<h_t*> player1_h      {&oh1,&oh2,&dh2,&oh2,&dh2,&oh2};
            std::vector<h_t*> player2_h      {&oh1,&dh1,&oh1,&oh1,&dh1,&dh2};
            std::vector<T(agent)*> player1_agent  {&mm,&uab1,&uab1,&uab1,&uab1,&uab1};
            std::vector<T(agent)*> player2_agent  {&uab2,&uab2,&uab2,&uab2,&uab2,&uab2};

            // define game instance
            game_t game;

            // generate the matchup data
            int num_plays = 20;
            for(int i = 0; i < player1_h.size(); ++i){
                int         p1sd= player1_sd[i];
                int         p2sd= player2_sd[i];
                h_t*        p1h = player1_h[i];
                h_t*        p2h = player2_h[i];
                T(agent)*   p1a = player1_agent[i];
                T(agent)*   p2a = player2_agent[i];

                // set agent parameters
                p1a->setMaxSearchDepth(p1sd);
                p1a->setUtilityEstimator(*p1h);
                p2a->setMaxSearchDepth(p2sd);
                p2a->setUtilityEstimator(*p2h);

                // add players to game
                game.addPlayer1(p1a);
                game.addPlayer2(p2a);

                // play game N times
                int nwin_p2 = 0;
                double tnexp[2] = {0}, nexpm[2] = {0}, pcapt[2]={0}, nmov[2]={0}, mtime[2] = {0};
                for(int n = 0; n < num_plays; ++n) {
                    game.play();
                    state_t gs = game.getFinalGameState();
                    gs.fprint(fout);
                    gs.print();
                    ch_print0("\n\n");
                    nwin_p2 += game.getPlayerWhoWon();

                    for(int k = 0; k < 2; ++k){
                        nmov[k] += game.getNumMoves(k)/static_cast<double>(num_plays);
                        pcapt[k] += game.getNumPiecesCapturedBy(k)/static_cast<double>(num_plays);
                        nexpm[k] += game.getAvgNumberNodesExpandedPerMoveFor(k)/static_cast<double>(num_plays);
                        tnexp[k] += game.getTotalNodesExpandedFor(k)/static_cast<double>(num_plays);
                        mtime[k] += game.getAvgMoveTimeFor(k)/static_cast<double>(num_plays);
                    }
                }

                // print statistics
                std::string a1n = p1a->name();
                std::string a2n = p2a->name();
                std::string h1n = p1h->name();
                std::string h2n = p2h->name();
                ch_print4("|| Matchup for %s (%s) vs %s (%s) ||\n------------------\n",
                          a1n.c_str(), h1n.c_str(), a2n.c_str(),h2n.c_str());

                // print win rate for each player
                ch_print1("Win rate for player 1 was: %lf%%\n",100.0*(num_plays - nwin_p2)/(double)(num_plays));
                ch_print1("Win rate for player 2 was: %lf%%\n",100.0*nwin_p2/(double)(num_plays));

                // print avg move time for players
                ch_print1("Avg move time for player 1 was: %lf ms\n",mtime[0]);
                ch_print1("Avg move time for player 2 was: %lf ms\n",mtime[1]);

                // print avg expanded nodes per move for player
                ch_print1("Avg expanded nodes per move for player 1 was: %lf nodes\n",nexpm[0]);
                ch_print1("Avg expanded nodes per move for player 2 was: %lf nodes\n",nexpm[1]);

                // get number of moves made by each player
                ch_print1("Number of moves to end of game by player 1 was %lf\n",nmov[0]);
                ch_print1("Number of moves to end of game by player 2 was %lf\n",nmov[1]);

                // get total number of nodes expanded by each player
                ch_print1("Total nodes expanded by player 1 is %lf\n",tnexp[0]);
                ch_print1("Total nodes expanded by player 2 is %lf\n",tnexp[1]);

                // total pieces captured by players
                ch_print1("Total pieces captured by player 1 is %lf\n",pcapt[0]);
                ch_print1("Total pieces captured by player 2 is %lf\n",pcapt[1]);

                //  add spaces
                ch_print0("\n\n\n\n\n");

            }


        }

#undef T(starter)

#define T(starter) starter##5x10n
        void matchups_5x10nominal() {

            // useful typedefs
            typedef T(pd_)::parent h_t;
            typedef T(bt_game) game_t;
            typedef game_t::state_t state_t;

            // define output file
            wrap::file fout("matchup_5x20n.txt",wrap::file::Write);

            // define agents that will be used
            T(bt_minimax)  mm;
            T(bt_uab)      uab1, uab2;

            // define heuristics
            T(pd_) dh1;
            T(po_) oh1;
            T(hd_) dh2;
            T(ho_) oh2;
            T(ld_) dh3;
            T(lo_) oh3;

            // define lists for matchup stuff
            std::vector<int> player1_sd(6,4); player1_sd[0] = 3;
            std::vector<int> player2_sd(6,4);
            std::vector<h_t*> player1_h      {&oh1,&oh2,&dh2,&oh2,&dh2,&oh2};
            std::vector<h_t*> player2_h      {&oh1,&dh1,&oh1,&oh1,&dh1,&dh2};
            std::vector<T(agent)*> player1_agent  {&mm,&uab1,&uab1,&uab1,&uab1,&uab1};
            std::vector<T(agent)*> player2_agent  {&uab2,&uab2,&uab2,&uab2,&uab2,&uab2};

            // define game instance
            game_t game;

            // generate the matchup data
            int num_plays = 20;
            for(int i = 0; i < player1_h.size(); ++i){
                int         p1sd= player1_sd[i];
                int         p2sd= player2_sd[i];
                h_t*        p1h = player1_h[i];
                h_t*        p2h = player2_h[i];
                T(agent)*   p1a = player1_agent[i];
                T(agent)*   p2a = player2_agent[i];

                // set agent parameters
                p1a->setMaxSearchDepth(p1sd);
                p1a->setUtilityEstimator(*p1h);
                p2a->setMaxSearchDepth(p2sd);
                p2a->setUtilityEstimator(*p2h);

                // add players to game
                game.addPlayer1(p1a);
                game.addPlayer2(p2a);

                // play game N times
                int nwin_p2 = 0;
                double tnexp[2] = {0}, nexpm[2] = {0}, pcapt[2]={0}, nmov[2]={0}, mtime[2] = {0};
                for(int n = 0; n < num_plays; ++n) {
                    game.play();
                    state_t gs = game.getFinalGameState();
                    gs.fprint(fout);
                    gs.print();
                    ch_print0("\n\n");
                    nwin_p2 += game.getPlayerWhoWon();

                    for(int k = 0; k < 2; ++k){
                        nmov[k] += game.getNumMoves(k)/static_cast<double>(num_plays);
                        pcapt[k] += game.getNumPiecesCapturedBy(k)/static_cast<double>(num_plays);
                        nexpm[k] += game.getAvgNumberNodesExpandedPerMoveFor(k)/static_cast<double>(num_plays);
                        tnexp[k] += game.getTotalNodesExpandedFor(k)/static_cast<double>(num_plays);
                        mtime[k] += game.getAvgMoveTimeFor(k)/static_cast<double>(num_plays);
                    }
                }

                // print statistics
                std::string a1n = p1a->name();
                std::string a2n = p2a->name();
                std::string h1n = p1h->name();
                std::string h2n = p2h->name();
                ch_print4("|| Matchup for %s (%s) vs %s (%s) ||\n------------------\n",
                          a1n.c_str(), h1n.c_str(), a2n.c_str(),h2n.c_str());

                // print win rate for each player
                ch_print1("Win rate for player 1 was: %lf%%\n",100.0*(num_plays - nwin_p2)/(double)(num_plays));
                ch_print1("Win rate for player 2 was: %lf%%\n",100.0*nwin_p2/(double)(num_plays));

                // print avg move time for players
                ch_print1("Avg move time for player 1 was: %lf ms\n",mtime[0]);
                ch_print1("Avg move time for player 2 was: %lf ms\n",mtime[1]);

                // print avg expanded nodes per move for player
                ch_print1("Avg expanded nodes per move for player 1 was: %lf nodes\n",nexpm[0]);
                ch_print1("Avg expanded nodes per move for player 2 was: %lf nodes\n",nexpm[1]);

                // get number of moves made by each player
                ch_print1("Number of moves to end of game by player 1 was %lf\n",nmov[0]);
                ch_print1("Number of moves to end of game by player 2 was %lf\n",nmov[1]);

                // get total number of nodes expanded by each player
                ch_print1("Total nodes expanded by player 1 is %lf\n",tnexp[0]);
                ch_print1("Total nodes expanded by player 2 is %lf\n",tnexp[1]);

                // total pieces captured by players
                ch_print1("Total pieces captured by player 1 is %lf\n",pcapt[0]);
                ch_print1("Total pieces captured by player 2 is %lf\n",pcapt[1]);

                //  add spaces
                ch_print0("\n\n\n\n\n");

            }
        }
#undef T(starter)

#define T(starter) starter##8x8e
        void matchups_8x8modified() {
            // useful typedefs
            typedef T(pd_)::parent h_t;
            typedef T(bt_game) game_t;
            typedef game_t::state_t state_t;

            // define output file
            wrap::file fout("matchup_8x8e.txt",wrap::file::Write);

            // define agents that will be used
            T(bt_minimax)  mm;
            T(bt_uab)      uab1, uab2;

            // define heuristics
            T(pd_) dh1;
            T(po_) oh1;
            T(hd_) dh2;
            T(ho_) oh2;
            T(ld_) dh3;
            T(lo_) oh3;

            // define lists for matchup stuff
            std::vector<int> player1_sd(6,4); player1_sd[0] = 3;
            std::vector<int> player2_sd(6,4);
            std::vector<h_t*> player1_h      {&oh1,&oh2,&dh2,&oh2,&dh2,&oh2};
            std::vector<h_t*> player2_h      {&oh1,&dh1,&oh1,&oh1,&dh1,&dh2};
            std::vector<T(agent)*> player1_agent  {&mm,&uab1,&uab1,&uab1,&uab1,&uab1};
            std::vector<T(agent)*> player2_agent  {&uab2,&uab2,&uab2,&uab2,&uab2,&uab2};

            // define game instance
            game_t game;

            // generate the matchup data
            int num_plays = 20;
            for(int i = 0; i < player1_h.size(); ++i){
                int         p1sd= player1_sd[i];
                int         p2sd= player2_sd[i];
                h_t*        p1h = player1_h[i];
                h_t*        p2h = player2_h[i];
                T(agent)*   p1a = player1_agent[i];
                T(agent)*   p2a = player2_agent[i];

                // set agent parameters
                p1a->setMaxSearchDepth(p1sd);
                p1a->setUtilityEstimator(*p1h);
                p2a->setMaxSearchDepth(p2sd);
                p2a->setUtilityEstimator(*p2h);

                // add players to game
                game.addPlayer1(p1a);
                game.addPlayer2(p2a);

                // play game N times
                int nwin_p2 = 0;
                double tnexp[2] = {0}, nexpm[2] = {0}, pcapt[2]={0}, nmov[2]={0}, mtime[2] = {0};
                for(int n = 0; n < num_plays; ++n) {
                    game.play();
                    state_t gs = game.getFinalGameState();
                    gs.fprint(fout);
                    gs.print();
                    ch_print0("\n\n");
                    nwin_p2 += game.getPlayerWhoWon();

                    for(int k = 0; k < 2; ++k){
                        nmov[k] += game.getNumMoves(k)/static_cast<double>(num_plays);
                        pcapt[k] += game.getNumPiecesCapturedBy(k)/static_cast<double>(num_plays);
                        nexpm[k] += game.getAvgNumberNodesExpandedPerMoveFor(k)/static_cast<double>(num_plays);
                        tnexp[k] += game.getTotalNodesExpandedFor(k)/static_cast<double>(num_plays);
                        mtime[k] += game.getAvgMoveTimeFor(k)/static_cast<double>(num_plays);
                    }
                }

                // print statistics
                std::string a1n = p1a->name();
                std::string a2n = p2a->name();
                std::string h1n = p1h->name();
                std::string h2n = p2h->name();
                ch_print4("|| Matchup for %s (%s) vs %s (%s) ||\n------------------\n",
                          a1n.c_str(), h1n.c_str(), a2n.c_str(),h2n.c_str());

                // print win rate for each player
                ch_print1("Win rate for player 1 was: %lf%%\n",100.0*(num_plays - nwin_p2)/(double)(num_plays));
                ch_print1("Win rate for player 2 was: %lf%%\n",100.0*nwin_p2/(double)(num_plays));

                // print avg move time for players
                ch_print1("Avg move time for player 1 was: %lf ms\n",mtime[0]);
                ch_print1("Avg move time for player 2 was: %lf ms\n",mtime[1]);

                // print avg expanded nodes per move for player
                ch_print1("Avg expanded nodes per move for player 1 was: %lf nodes\n",nexpm[0]);
                ch_print1("Avg expanded nodes per move for player 2 was: %lf nodes\n",nexpm[1]);

                // get number of moves made by each player
                ch_print1("Number of moves to end of game by player 1 was %lf\n",nmov[0]);
                ch_print1("Number of moves to end of game by player 2 was %lf\n",nmov[1]);

                // get total number of nodes expanded by each player
                ch_print1("Total nodes expanded by player 1 is %lf\n",tnexp[0]);
                ch_print1("Total nodes expanded by player 2 is %lf\n",tnexp[1]);

                // total pieces captured by players
                ch_print1("Total pieces captured by player 1 is %lf\n",pcapt[0]);
                ch_print1("Total pieces captured by player 2 is %lf\n",pcapt[1]);

                //  add spaces
                ch_print0("\n\n\n\n\n");

            }
        }
#undef T(starter)
    }
}
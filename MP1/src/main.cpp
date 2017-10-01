/*
 *  main.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/14/17
 *  Purpose:
 *
 */


#include <stdio.h>
#include "MessageException.hpp"
#include "custom_exception.hpp"
#include "text_color.hpp"
#include "maze_io.hpp"
#include "maze.hpp"
#include "commandline_parser.hpp"
#include "astar_planner.hpp"
#include "astar_manhattan.hpp"
#include "astar_euclidean.hpp"
#include "astar_deviation.hpp"
#include "astar_average.hpp"
#include "astar_nearest.hpp"
#include "dfs_planner.hpp"
#include "bfs_planner.hpp"
#include "astar_convexhull.hpp"
#include "astar_convexhull_fast.hpp"
#include <chrono>
#include "gif_wrapper.hpp"
#include "maze_transition_model.hpp"
#include "multi_state.hpp"
#include "sokoban_transition_model.hpp"
#include "astar_sokoban_nearest.hpp"


#ifndef _WIN32
#define LIBPNG_DEFINED
#endif


#ifdef LIBPNG_DEFINED
void writeGifSolution( std::map<wrap::tile_t, img::image> & tile_set,
                       const std::string & gif_path,
                       const maze & maze_,
                       const path & path );
#endif


int main(int argc, char** argv){
    
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    try {
        
        // get command line inputs
        parser::commandline commp(argc,argv);
        std::string maze_file   = commp["-maze"];
        std::string agent_tile  = commp["-agent_t"];
        std::string wall_tile   = commp["-wall_t"];
        std::string back_tile   = commp["-back_t"];
        std::string box_tile    = commp["-box_t"];
        std::string goal_tile   = commp["-goal_t"];
        std::string out_gif     = commp["-out_gif"];
        std::string heuristic_t = commp["-h"];
        std::string planner     = commp["-p"];
        std::string out_maze_file = commp["-out"];
        std::string stats_file  = commp["-stats"];
        
        
        // load maze
        maze maze_;
        if( !maze_file.empty() ){
            maze_io::load_maze(maze_file, maze_);
        }else{
            throw custom::exception("Did not pass in a maze file. This is the commandline argument `-maze`. Try again.");
        }
        bool isSokoban = maze_.numBoxes() > 0;
        
        // define planners
        bfs::planner<transition::maze_model> bplanner;
        bfs::planner<transition::sokoban_model> bplanner_s;
        dfs::planner dplanner;
        astar::planner<transition::maze_model> aplanner;
        astar::planner<transition::sokoban_model> aplanner_s;
        path_planner* planner_ = nullptr;
        
        // define heuristic function
        astar::manhattan_dist   h_m;    h_m.setMaze(maze_);
        astar::euclidean_dist   h_e;    h_e.setMaze(maze_);
        astar::deviation        h_d;    h_d.setMaze(maze_); h_d.setScaleFactor(0.55);
        astar::average          h_a;    h_a.setMaze(maze_); h_a.setScaleFactor(1.0);
        astar::convexhull       h_ch;   h_ch.setMaze(maze_);
        astar::fast::convexhull h_chf;  h_chf.setMaze(maze_);
        astar::nearest          h_n;    h_n.setMaze(maze_);
        sokoban::nearest        h_ns;   h_ns.setMaze(maze_);
        astar::heuristic_func_base<multi::state>* heuristic = nullptr;
        astar::heuristic_func_base<sokoban::state>* heuristic_s = nullptr;
        
        // set the planner method
        bool usesHeuristic = false;
        if( planner == "bfs" )          {
            planner_ = &bplanner;
            if( isSokoban ){ planner_ = &bplanner_s; }
        }
        else if( planner == "dfs" )     { planner_ = &dplanner; }
        else if( planner == "greedy" )  { planner_ = &aplanner; aplanner.beGreedy(true); usesHeuristic = true;}
        else if( planner == "astar" )   {
            planner_ = &aplanner;
            if( isSokoban ){ planner_ = &aplanner_s; }
            usesHeuristic = true;
        }
        else{
            throw custom::exception("Did not specify valid planner type. This is the `-p` commandline argument. Options for argument value are:\nbfs: Breadth First Search\ndfs: Depth First Search\ngreedy: Greedy\nastar: A*");
        }
        
        // set the heuristic function, if necessary
        if( usesHeuristic ){
            if( heuristic_t == "mdist" )          {heuristic = &h_m;}
            else if( heuristic_t == "edist" )     {heuristic = &h_e;}
            else if( heuristic_t == "avg" )       {heuristic = &h_a;}
            else if( heuristic_t == "chull" )     {heuristic = &h_chf;}
            else if( heuristic_t == "nearest" )   {
                heuristic   = &h_n;
                heuristic_s = &h_ns;
            }
            else{
                throw custom::exception("Did not specify valid heuristic. This is the `-h` commandline argument. Options for argument value are:"
                                        "\nmdist: Manhattan Distance Heuristic"
                                        "\nedist: Euclidean Distance Heuristic"
                                        "\navg  : Average Distance Heuristic"
                                        "\nchull: Convex Hull Heuristic"
                                        "\nnearest: Nearest Goal Heuristic");
            }
            aplanner.setHeuristic(*heuristic);
            aplanner_s.setHeuristic(*heuristic_s);
        }
        
#ifdef LIBPNG_DEFINED
        // load tile images
        bool didLoadAllTiles = true;
        std::map<wrap::tile_t, img::image> image_set;
        image_set[wrap::Background] = img::image( back_tile );
        image_set[wrap::Wall]       = img::image( wall_tile );
        image_set[wrap::Agent]      = img::image( agent_tile );
        image_set[wrap::Goal]       = img::image( goal_tile );
        image_set[wrap::Box]        = img::image( box_tile );
        if( back_tile.empty() || wall_tile.empty() || agent_tile.empty() || goal_tile.empty() || (isSokoban && box_tile.empty())){
            printf("Did not pass in tiles so you can generate GIF. The tile inputs are:\n"
                                    "wall_t : Wall barrier tile\n"
                                    "back_t : Background/Floor tile\n"
                                    "agent_t: AI Agent tile\n"
                                    "goal_t : Goal point tile\n"
                                    "box_t  : Box tile\n"
                                    "Continuing...\n");
            didLoadAllTiles = false;
        }
        
#endif
        
        // define path variable
        path path;
        
        // compute maze solution using path planning algorithm
        planner_->computePath(maze_, path);
        path.printResults();
        
        // save statistics to file
        if( stats_file.empty() ){ stats_file = "out_stats.txt"; }
        path.writeResults(stats_file);
        
        // save solved maze to file
        if( out_maze_file.size() == 0 ){ out_maze_file = "out_maze.txt"; }
        maze_io::save_maze(out_maze_file, maze_, &path);
        
#ifdef LIBPNG_DEFINED
        // generate GIF of the maze
        if( didLoadAllTiles ){
            if( out_gif.size() == 0 ){ out_gif = "out_gif.gif"; }
            writeGifSolution(image_set, out_gif, maze_, path);
        }
#endif
        
    
    }catch( MessageException & msg ){
        text::printf_color(text::Cyan, "Exception: ");
        msg.msg();
        text::printf_color(text::Red, "\n");
        return -1;
        
    }catch( std::exception & msg ){
        text::printf_color(text::Cyan, "Exception: ");
        text::printf_color(text::Red, msg.what());
        text::printf_color(text::Red, "\n");
        return -2;
    }
    
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    auto time_span = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    printf("The code ran for %lf seconds\n", (double)(time_span.count()/1000.0) );
    
	return 0;
}

#ifdef LIBPNG_DEFINED

bool inList( maze::id_type id, const std::vector<maze::id_type> & list ){
    for(unsigned int i = 0; i < list.size(); ++i){
        if( id == list[i] ){ return true; }
    }
    return false;
}

maze::Action figureOutAction( maze::id_type cid, maze::id_type lid, int ncols ){
    int delta = static_cast<int>(cid) - static_cast<int>(lid);
    if      ( delta == 1        )   { return maze::Right;   }
    else if ( delta == -1       )   { return maze::Left;    }
    else if ( delta == ncols    )   { return maze::Down;    }
    else if ( delta == -ncols   )   { return maze::Up;      }
    else{
        return maze::Null;
    }
}

void writeGifSolution( std::map<wrap::tile_t, img::image> & tile_set,
                       const std::string & gif_path,
                       const maze & maze_,
                       const path & path )
{
    // figure out if we solved a Sokoban problem
    bool isSokoban = maze_.numBoxes() > 0;
    std::vector<maze::id_type> box_positions = maze_.getBoxPositions();
    
    // get size of maze
    unsigned int mw = maze_.getNumCols();
    unsigned int mh = maze_.getNumRows();
    unsigned int iw = 50;
    unsigned int ih = 50;
    
    // initialize image that frame will be drawn onto
    img::image img_;
    {
        const auto & timg = tile_set[wrap::Background];
        iw = timg.width(); ih = timg.height();
        img_.setDims(mw*iw, mh*ih);
    }
    
    // init gif
    wrap::gif gif_instance(img_.width(),img_.height());
    gif_instance.init(gif_path);
    
    // loop through and create each frame
    auto path_list = path.path_list;
    std::map<unsigned int, bool> goal_visited;
    bool firstImage = true;
    unsigned int lid = 0; // last id
    while(!path_list.empty()) {
        auto current_id = path_list.front(); path_list.pop_front();
        if( !isSokoban && maze_.idIsGoalPoint(current_id) ){ goal_visited[current_id] = true; }
        
        if( firstImage ){
            firstImage = false;
            lid = current_id;
        }else{
            
            if( isSokoban ){
                
                // update an applicable box position if needed
                for(unsigned int i = 0; i < box_positions.size(); ++i){
                    if( box_positions[i] == current_id ){
                        // figure out action based on last id and current id
                        maze::Action a = figureOutAction(current_id, lid, mw);
                        
                        // update box position based on action
                        box_positions[i] = transition::sokoban_model::newMazePos(current_id, a, &maze_);
                        break;
                    }
                }// end for
                
            }// end if isSokoban
            lid = current_id;
        }
        
        unsigned int id = 0;
        for(unsigned int i = 0; i < mw; ++i){
            for(unsigned int j = 0; j < mh; ++j){
                id = i + j*mw;
                if( id == current_id ){
                    img_.insertSubImage(i*iw, j*ih, tile_set[wrap::Agent]);
                }else if( isSokoban && inList(id,box_positions) ){
                        img_.insertSubImage(i*iw, j*ih, tile_set[wrap::Box]);
                }else if( maze_.idIsGoalPoint(id) && goal_visited.find(id) == goal_visited.end() ){
                    img_.insertSubImage(i*iw, j*ih, tile_set[wrap::Goal]);
                }else if( maze_.getValidityAtLocationID(id) ){
                    img_.insertSubImage(i*iw, j*ih, tile_set[wrap::Background]);
                }else{
                    img_.insertSubImage(i*iw, j*ih, tile_set[wrap::Wall]);
                }
            }// end for j
        }// end for i
        
        // save off image to gif
        gif_instance.addFrame(img_);
    }// end while
}
#endif

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
#include "dfs_planner.hpp"
#include "bfs_planner.hpp"
#include "astar_convexhull.hpp"
#include <chrono>
#include "gif_wrapper.hpp"
#include "maze_transition_model.hpp"
#include "multi_state.hpp"


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
        std::string maze_file = commp["-maze"];
        std::string agent_tile = commp["-agent_t"];
        std::string wall_tile = commp["-wall_t"];
        std::string back_tile = commp["-back_t"];
        std::string goal_tile = commp["-goal_t"];
        std::string out_gif   = commp["-out_gif"];
        
#ifdef LIBPNG_DEFINED
        // load tile images
        std::map<wrap::tile_t, img::image> image_set;
        image_set[wrap::Background] = img::image( back_tile );
        image_set[wrap::Wall]       = img::image( wall_tile );
        image_set[wrap::Agent]      = img::image( agent_tile );
        image_set[wrap::Goal]       = img::image( goal_tile );
#endif
        
        // load maze
        maze maze_;
        maze_io::load_maze(maze_file, maze_);
        
        
        // define path variable
        path path1, path2, path3;
        
        // define heuristic function
        astar::manhattan_dist   h_m;    h_m.setMaze(maze_);
        astar::euclidean_dist   h_e;    h_e.setMaze(maze_);
        astar::deviation        h_d;    h_d.setMaze(maze_); h_d.setScaleFactor(0.55);
        astar::average          h_a;    h_a.setMaze(maze_); h_a.setScaleFactor(1.0);
        astar::convexhull       h_ch;   h_ch.setMaze(maze_); h_ch.setScaleFactor(20); // 2.5 leads to 207 cost for mediumSearch
        
        // define bfs planner
        bfs::planner bplanner;
        
        //define dfs planner
        dfs::planner dplanner;
        
        // define planning algorithm and set heuristic function
        astar::planner<transition::maze_model> aplanner;
        aplanner.setHeuristic(h_ch);
        
        //use greedy heuristic
        //aplanner.setHeuristic(h, true);
        
        // compute maze solution using path planning algorithm
        aplanner.computePath(maze_, path1);
        //bplanner.computePath(maze_, path1);
        //dplanner.computePath(maze_, path1);
        path1.printResults();
        
        
        // save solved maze to file
        std::string out_maze_file = commp["-out"];
        if( out_maze_file.size() == 0 ){ out_maze_file = "out_maze.txt"; }
        maze_io::save_maze(out_maze_file, maze_, &path1);
        
#ifdef LIBPNG_DEFINED
        // generate GIF of the maze
        //if( out_gif.size() == 0 ){ out_gif = "out_gif.gif"; }
        //writeGifSolution(image_set, out_gif, maze_, path1);
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
    printf("The code ran for %lu milliseconds\n",time_span.count());
    
	return 0;
}

#ifdef LIBPNG_DEFINED
void writeGifSolution( std::map<wrap::tile_t, img::image> & tile_set,
                       const std::string & gif_path,
                       const maze & maze_,
                       const path & path )
{
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
    while(!path_list.empty()) {
        auto current_id = path_list.front(); path_list.pop_front();
        if( maze_.idIsGoalPoint(current_id) ){ goal_visited[current_id] = true; }
        unsigned int id = 0;
        for(unsigned int i = 0; i < mw; ++i){
            for(unsigned int j = 0; j < mh; ++j){
                id = i + j*mw;
                if( id == current_id ){
                    img_.insertSubImage(i*iw, j*ih, tile_set[wrap::Agent]);
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

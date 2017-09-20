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

int main(int argc, char** argv){
    
    try {
        
        // load maze input
        parser::commandline commp(argc,argv);
        std::string maze_file = commp["-maze"];
        maze maze_;
        maze_io::load_maze(maze_file, maze_);
        
        // define path variable
        path path1, path2, path3;
        
        // define heuristic function
        astar::manhattan_dist   h_m;    h_m.setMaze(maze_);
        astar::euclidean_dist   h_e;    h_e.setMaze(maze_);
        astar::deviation        h_d;    h_d.setMaze(maze_); h_d.setScaleFactor(0.55);
        astar::average          h_a;    h_a.setMaze(maze_); h_a.setScaleFactor(0.55);
        
        // define dfs planner
        dfs::planner dplanner;
        
        // define planning algorithm and set heuristic function
        astar::planner aplanner;
        aplanner.setHeuristic(h_a);
        
        //use greedy heuristic
//        aplanner.setHeuristic(h, true);
        
        // compute maze solution using path planning algorithm
        aplanner.computePath(maze_, path1);
        path1.printResults();
        
        // save solved maze to file
        std::string out_maze_file = commp["-out"];
        if( out_maze_file.size() == 0 ){ out_maze_file = "out_maze.txt"; }
        maze_io::save_maze(out_maze_file, maze_, &path1);
    
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
    
	return 0;
}

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

int main(int argc, char** argv){
    
    try {
        
        // load maze input
        parser::commandline commp(argc,argv);
        std::string maze_file = commp["-maze"];
        maze maze_;
        maze_io::load_maze(maze_file, maze_);
        auto sid = maze_.getStartingLocationID();
        auto actions = maze_.getActionSetForID(sid-2);
        
        // save maze to file and see that it matches original
        maze_io::save_maze("testmaze.txt", maze_);
    
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

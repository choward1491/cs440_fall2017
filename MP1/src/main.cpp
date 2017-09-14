/*
**
** Author: C. Howard
**
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
        parser::commandline commp(argc,argv);
        std::string maze_file = commp["-maze"];
        maze maze_;
        maze_io::load_maze(maze_file, maze_);
        maze::maze_graph & mgraph = maze_.getGraph();
        auto & list = mgraph.getConnectivityListFor(0);
        maze_io::save_maze("/Users/cjh/Documents/testmaze.txt", maze_);
    
    }catch( MessageException & msg ){
        text::printf_color(text::Cyan, "Exception: ");
        msg.msg();
        return -1;
        
    }catch( std::exception & msg ){
        text::printf_color(text::Cyan, "Exception: ");
        text::printf_color(text::Yellow, msg.what());
        return -2;
    }
    
	return 0;
}

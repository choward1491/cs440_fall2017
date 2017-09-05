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
#include "commandline_parser.hpp"

int main(int argc, char** argv){
    
    try {
        parser::commandline commp(argc,argv);
        std::string maze_file = commp["-maze"];
        maze_io::maze_graph mgraph;
        maze_io::id_list    fpoints;
        maze_io::maze_graph::id_type start_id = 0;
        maze_io::load_maze(maze_file, mgraph, fpoints, start_id);
        auto & list = mgraph.getConnectivityListFor(0);
    
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

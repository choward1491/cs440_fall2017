
/* 
 * File:   flow_io.cpp
 * Author: lukep
 * 
 * Created on October 24, 2017, 2:27 AM
 */

#include <vector>
#include <iostream>
#include "stdio.h"
#include "flow_io.h"
#include "flow_csp.h"
#include "FileWrap.hpp"


void flow_io::loadFlow(const std::string& flow_file, csp::flow_csp& out_flow) {
    // open flow file
    wrap::file fl(flow_file,wrap::file::Read);

    // process flow file if applicable
    if( fl.isOpen() ){
        int rows = 1, cols = -1;
//        std::set<char> colors;
        std::set<enum csp::flow_csp::domains::domain_type> colors;
        char c;
        
        // get number of columns
        while( fscanf(fl,"%c",&c) != EOF && c != '\n' ){ cols++; }
        // get number of rows for grid
        while( fscanf(fl,"%*[^\n]\n") != EOF ){ rows++; }
        std::cout << cols << " by " << rows << std::endl;
        // allocate storage
        //char grid[cols*rows];
        std::cout << "here12123";
        std::vector<char> grid;
        std::cout << "here9";
        // reset maze file and dump maze into map array
        fl.resetFile();
        int ind = 0;
        while( fscanf(fl,"%c",&c) != EOF ){
            std::cout << "here1";
            if(c != '\n' && c != '\r') {
                grid[ind] = c;
                if(c != '_') {
                    std::cout << "here";
                    csp::flow_csp::domains::domain_type dm = static_cast<csp::flow_csp::domains::domain_type>(c);
                    colors.insert(dm);
                    std::cout << "here2";
                }
                ind++;
            }
        } // end while
//        out_flow

        for(int i = 0; i < rows; i ++) {
            for(int j=0; j < cols; j++) {
                std::cout << grid[i*8+j];
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "Bad flow file" << std::endl;
    }
}
void flow_io::saveFlow(const std::string& flow_file, const csp::flow_csp& in_flow) {

}

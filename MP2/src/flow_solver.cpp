
/* 
 * File:   flow_solver.cpp
 * Author: lukep
 * 
 * Created on October 26, 2017, 9:32 PM
 */

#include <vector>
#include <iostream>
#include "stdio.h"
#include "flow_solver.h"
#include "FileWrap.hpp"



void flow_solver::printCurrent() {
    std::cout << "Assignment: " << std::endl;
    for(int i = 0; i < ncol; i++) {
        for(int j = 0; j < nrow; j++) {
            domain_type dd = assignmentGrid[i*ncol + j];
            std::cout << dmap[dd] << " ";
        }
        std::cout << std::endl;
    }
}


void flow_solver::loadFlow(const std::string& flow_file) {
    // open flow file
    wrap::file fl(flow_file,wrap::file::Read);

    // process flow file if applicable
    if( fl.isOpen() ){
        int rows = 1, cols = -1;
        std::set<enum domain_type> colors;
        char c;
        
        // get number of columns
        while( fscanf(fl,"%c",&c) != EOF && c != '\n' ){ cols++; }
        // get number of rows for grid
        while( fscanf(fl,"%*[^\n]\n") != EOF ){ rows++; }
        std::cout << cols << " by " << rows << std::endl;
        
        // allocate storage
        ncol = cols; nrow = rows;
        std::vector<char> grid(cols*rows, 0);
        domainGrid.resize(cols*rows);
        assignmentGrid.resize(cols*rows, NONE);
        assigned.resize(cols*rows, false);
        
        // reset maze file and dump maze into char grid
        fl.resetFile();
        int ind = 0;
        while( fscanf(fl,"%c",&c) != EOF ){
            if(c != '\n' && c != '\r') {
                grid[ind] = c;
                if(c != '_') {
                    domain_type dm = static_cast<domain_type>(c-65);
                    colors.insert(dm);
                }
                ind++;
            }
        } // end while
        
        // setup domains
        for(int j = 0; j < (int)grid.size(); j++) {
            if(grid[j] == '_') {
                domainGrid[j] = colors;
            } else {
                std::set<enum domain_type> domaintmp;
                domain_type dom = static_cast<domain_type>(grid[j]-65);
                domainGrid[j] = domaintmp;
                assignmentGrid[j] = dom;
                assigned[j] = true;
                isSource[j] = true;
            }
        }
        
    } else {
        std::cout << "Bad flow file" << std::endl;
    }
}

int flow_solver::getUnvisitedVariable() {
    if(!beSmart) {
        for(int i = 0; i < assigned.size(); i++) {
            if(assigned[i] == false) {
                return i;
            }
        }
    } else {
        // TODO: better var selection
    }
    
    return -1; // no more to assign
}


void flow_solver::saveFlow(const std::string& flow_file) {
    printCurrent();
}

std::vector<flow_solver::domain_type> flow_solver::getOrderedDomain(int ind) {
    std::vector<domain_type> ret;
    if(beSmart) {
        // TODO better domain ordering
    } else {
        std::set<enum domain_type> subDom = domainGrid[ind];
        std::copy(subDom.begin(), subDom.end(), std::back_inserter(ret));
    }
    return ret;
}

bool flow_solver::isConsistent(int var, domain_type val) {
    bool ret = true;
    
    // check adjacent
    
    return ret;
}


bool flow_solver::solve() {
//    std::cout << "recurse" << std::endl;
    printCurrent();
    bool success = false;
    int var = getUnvisitedVariable();
    std::cout << "var: " << var << std::endl;
    if(var == -1) return true;
    
    // try all possible values
    for(auto value : getOrderedDomain(var)) {
        if(isConsistent(var, value)) {
            assigned[var] = true;
            assignmentGrid[var] = value;
            
            // TODO Perform inference and early fail here
            
            success = solve();
            if(success) return success;
            assigned[var] = false;
            assignmentGrid[var] = NONE;
        }
    }
    
    return success; // ???
}

void flow_solver::setSmart(bool beSmart) {
    this->beSmart = beSmart;
}


flow_solver::flow_solver() {
    dmap[flow_solver::domain_type::A] = 'A';
    dmap[flow_solver::domain_type::B] = 'B';
    dmap[flow_solver::domain_type::C] = 'C';
    dmap[flow_solver::domain_type::D] = 'D';
    dmap[flow_solver::domain_type::E] = 'E';
    dmap[flow_solver::domain_type::F] = 'F';
    dmap[flow_solver::domain_type::G] = 'G';
    dmap[flow_solver::domain_type::H] = 'H';
    dmap[flow_solver::domain_type::I] = 'I';
    dmap[flow_solver::domain_type::J] = 'J';
    dmap[flow_solver::domain_type::K] = 'K';
    dmap[flow_solver::domain_type::L] = 'L';
    dmap[flow_solver::domain_type::M] = 'M';
    dmap[flow_solver::domain_type::N] = 'N';
    dmap[flow_solver::domain_type::O] = 'O';
    dmap[flow_solver::domain_type::P] = 'P';
    dmap[flow_solver::domain_type::Q] = 'Q';
    dmap[flow_solver::domain_type::R] = 'R';
    dmap[flow_solver::domain_type::S] = 'S';
    dmap[flow_solver::domain_type::T] = 'T';
    dmap[flow_solver::domain_type::U] = 'U';
    dmap[flow_solver::domain_type::V] = 'V';
    dmap[flow_solver::domain_type::W] = 'W';
    dmap[flow_solver::domain_type::X] = 'X';
    dmap[flow_solver::domain_type::Y] = 'Y';
    dmap[flow_solver::domain_type::Z] = 'Z';
    dmap[flow_solver::domain_type::NONE] = '-';
}

flow_solver::flow_solver(const flow_solver& orig) {
}

flow_solver::~flow_solver() {
}


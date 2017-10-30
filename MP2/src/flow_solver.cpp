
/* 
 * File:   flow_solver.cpp
 * Author: lukep
 * 
 * Created on October 26, 2017, 9:32 PM
 */

#include <vector>
#include <iostream>
#include <fstream>
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

void flow_solver::firstRestrict() {
    for(int i = 0; i < ncol*nrow; i++) {
        if(isSource[i]) {
            std::vector<int> neighbors = getNeighbors(i);
            int noneCount = 0, noneInd = -1;
            for(int n : neighbors) {
                if(assigned[n] == false) {
                    noneCount++;
                    noneInd = n;
                }
            }
            if(noneCount == 1) {
                auto dkeep = assignmentGrid[i];
//                printf("ind:ind %d keep: %d\n",noneInd,dkeep);
                domainGrid[noneInd].clear();
                domainGrid[noneInd].insert(dkeep);
//                printf("restricted: %d, s: %d\n",noneInd,domainGrid[noneInd].size());
            }
        }
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
//        std::cout << cols << " by " << rows << std::endl;
        
        // allocate storage
        ncol = cols; nrow = rows;
        int dimension = cols*rows;
        std::vector<char> grid(dimension, 0);
        domainGrid.resize(dimension);
        assignmentGrid.resize(dimension);
        assigned.resize(dimension);
        isSource.resize(dimension);
        for(int i = 0; i < dimension; i++) {
            assigned[i] = false;
            isSource[i] = false;
            assignmentGrid[i] = NONE;
        }
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
        
//        for(int k = 0; k < domainGrid.size(); k++) {
//            printf("%d: %d\n",k,domainGrid[k].size());
//            for(auto d : domainGrid[k]) {
//                printf("%d, ",d);
//            }
//            printf("\n");
//        }
        
        // initial restriction
        firstRestrict();
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
    } else { // MRV
        int min = 1000;
        int current = -1;
        for(int i = 0; i < assigned.size(); i++) {
            if(assigned[i] == false) {
                int remaining = domainGrid[i].size();
                if(remaining < min) {
                    min = remaining;
//                    printf("min: %d, var: %d\n",min,i);
                    current = i;
                }
            }
        }
        return current;
    }
    return -1; // no more to assign
}

void flow_solver::saveFlow(const std::string& flow_file) {
    printCurrent();
    std::ofstream outFile;
    outFile.open(flow_file);
    for(int i = 0; i < ncol; i++) {
        for(int j = 0; j < nrow; j++) {
            domain_type dd = assignmentGrid[i*ncol + j];
            outFile << dmap[dd];
        }
        outFile << "\r\n";
    }
}

std::vector<flow_solver::domain_type> flow_solver::getOrderedDomain(int ind) {
    std::vector<domain_type> ret;
    if(beSmart) {
        std::set<enum domain_type> subDom = domainGrid[ind];
        std::copy(subDom.begin(), subDom.end(), std::back_inserter(ret));
    } else {
        std::set<enum domain_type> subDom = domainGrid[ind];
        std::copy(subDom.begin(), subDom.end(), std::back_inserter(ret));
    }
    return ret;
}

std::vector<int> flow_solver::getNeighbors(int var) {
    std::vector<int> ret;
    int a, b;
    a = var/ncol;
    b = var - (a*ncol);
    if(b > 0) ret.push_back(a*ncol+(b-1));
    if((b+1) < ncol) ret.push_back(a*ncol+(b+1));
    if(a > 0) ret.push_back((a-1)*ncol + b);
    if((a+1) < nrow) ret.push_back((a+1)*ncol + b);
    return ret;
}

bool flow_solver::checkNeighborOver(int var, domain_type val) {
    std::vector<int> neighbors = getNeighbors(var);
    int sameCount = 0;
    for(int n : neighbors) {
        if(assignmentGrid[n] == val) {
            sameCount++;
        }
    }
    int threshold = 1 - ((isSource[var])? 1 : 0);
    return !(sameCount > threshold); // 3+ neighbors of same color
}

bool flow_solver::sourceCheck(int var, domain_type val, int ignorevar) {
    std::vector<int> neighbors = getNeighbors(var);
    int sameCount = 0;
    int noneCount = 0;
    if(assignmentGrid[var] == val) sameCount++;
    for(int n : neighbors) {
        if(assignmentGrid[n] == NONE && n != ignorevar) noneCount++;
        if(assignmentGrid[n] == assignmentGrid[var]) {
            sameCount++;
        }
    }
    if(noneCount > 0) return true;
    return (sameCount == 1); // Source must have 1 neighbor
}

bool flow_solver::isConsistent(int var, domain_type val) {
//    if(beSmart) {
        
//    } else {
        std::vector<int> neighbors = getNeighbors(var);
        int sameCount = 0, noneCount = 0;
        for(int n : neighbors) {
            if(isSource[n]) if(sourceCheck(n,val,var) == false) return false;
            if(assignmentGrid[n] == NONE) noneCount++;
            if(assignmentGrid[n] == val) {
                sameCount++;
                if(checkNeighborOver(n, val) == false) return false;
            }
        }
        if(sameCount > 2) return false; // 3+ neighbors of same color
        if(noneCount == 0) {
            if(sameCount != 2) return false;
        }
//    }
    
    return true;
}

bool flow_solver::lastCheck() {
//    printf("last: %d\n",attempts);
    for(int i = 0; i < ncol*nrow; i++) {
        std::vector<int> neighbors = getNeighbors(i);
        int sameCount = 0;
        for(int n : neighbors) {
            if(assignmentGrid[n] == assignmentGrid[i]) {
                sameCount++;
            }
        }
        int threshold = 2 - ((isSource[i])? 1 : 0);
        if(sameCount != threshold) return false;
    }
    return true;
}

bool flow_solver::solve() {
//    printCurrent();
    bool success = false;
    int var = getUnvisitedVariable();
//    std::cout << "svar: " << var << std::endl;
    if(var == -1) { //all assigned
        return lastCheck();
    }
    // try all possible values
    for(auto value : getOrderedDomain(var)) {
        if(isConsistent(var, value)) {
            assigned[var] = true;
            assignmentGrid[var] = value;
            attempts++;
            
//            if(attempts > 50) return true;
            // TODO Perform inference and early fail here
            
            success = solve();
            if(success) return success;
            assigned[var] = false;
            assignmentGrid[var] = NONE;
        }
    }
//    printf("why\n");
    return success; // needed???
}

void flow_solver::setSmart(bool beSmart) {
    this->beSmart = beSmart;
}


flow_solver::flow_solver() {
    attempts = 0;
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

int flow_solver::getAttempts() {
    return attempts;
}

flow_solver::flow_solver(const flow_solver& orig) {
}

flow_solver::~flow_solver() {
}


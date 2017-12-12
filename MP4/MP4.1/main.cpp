/* 
 * File:   main.cpp
 * Author: Luke Pitstick
 * Copyright 2017 Luke Pitstick
 * Created on November 27, 2017, 5:27 PM
 */

#include <cstdlib>
#include <iostream>
#include <chrono>
#include "digitSolver.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    double setdecay = 1.0;
    bool setbias = false, setinitalzero = true, setrandomorder = false;
    int setepochs = 100;
    
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    
    // digit identification   
    try {
        digitSolver ds("testimages","testlabels","trainingimages","traininglabels", 
                setdecay, setbias, setinitalzero, setrandomorder, setepochs);
        ds.solve();
    } catch(exception &e) {
        cout << "error: " << e.what();
    }
    
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    auto time_span = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    printf("The code ran for %lf seconds\n", (time_span.count()/1000.0) );
            
    return 0;
}


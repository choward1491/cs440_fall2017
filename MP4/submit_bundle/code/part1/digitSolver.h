
/* 
 * File:   digitSolver.h
 * Author: lukep
 *
 * Created on November 27, 2017, 5:29 PM
 */

#ifndef DIGITSOLVER_H
#define DIGITSOLVER_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class digitSolver {
public:
    digitSolver();
    digitSolver(string testImages, string testLabels, string trainingImages, string trainingLables, 
                double setdecay, bool setbias, bool setinitalzero, bool setrandomorder, int setepochs);
    digitSolver(const digitSolver& orig);
    virtual ~digitSolver();
    void setWeights();
    void test();
    void solve();
private:
    string testImages, testLabels, trainingImages, trainingLabels;
    double setdecay;
    bool setbias, setinitalzero, setrandomorder;
    int setepochs;
    vector<vector<double>> weights;
    vector<double> biases;
    
    double dotProduct(const vector<double>& a, const vector<double>& b);
    void updateWeight(int wi, vector<double> img, double alpha, bool add); 
};

#endif /* DIGITSOLVER_H */


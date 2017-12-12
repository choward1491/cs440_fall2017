
/* 
 * File:   knnSolver.h
 * Author: lukep
 *
 * Created on December 11, 2017, 9:08 PM
 */

#ifndef KNNSOLVER_H
#define KNNSOLVER_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class knnSolver {
public:
    knnSolver();
    knnSolver(string testImages, string testLabels, string trainingImages, string trainingLabels, int k);
    knnSolver(const knnSolver& orig);
    virtual ~knnSolver();
    void load();
    void test();
    void solve();
private:
    string testImages, testLabels, trainingImages, trainingLabels;
    vector<int> trainLabels;
    vector<vector<int>> coordinates;
    int k;
    int distance(const vector<int>& a, const vector<int>& b);
};

#endif /* KNNSOLVER_H */


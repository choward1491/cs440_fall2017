
/* 
 * File:   knnSolver.cpp
 * Author: lukep
 * 
 * Created on December 11, 2017, 9:08 PM
 */


#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <ctime>
#include <random>
#include <algorithm>
#include "knnSolver.h"

#define featureCount 784
#define testCount 1000
#define trainCount 5000
#define classCount 10
#define width 28
#define height 28

using namespace std;

int knnSolver::distance(const vector<int>& a, const vector<int>& b) {
    int dist = 0;
    for(int i = 0; i < featureCount; i++) {
        dist += abs(a[i] - b[i]);
    }
    return dist;
}

// train the weights in epochs
void knnSolver::load() {
    vector<int> trainExamplesCount;
    
    ifstream trainImgs, trainLabel;
    
    
    trainExamplesCount.resize(classCount, 0);
    
    trainImgs.open(trainingImages);
    trainLabel.open(trainingLabels);
    
    //read values into coordinates
    int label = 0, ind = 0;
    if(trainLabel.is_open() && trainImgs.is_open()) {
        for(ind; ind < trainCount; ind++) {
            char val;            
            
            trainLabel >> label;
            trainExamplesCount[label] += 1; // count occurrences
            trainLabels[ind] = label;
            
            for(int i = 0; i < height; i++) {
                for(int j = 0; j <= width; j++) {
                    trainImgs.get(val);
                    if(val == '#' || val == '+') {
                        coordinates[ind][i*height+j] = 1;
                    } else if(val == ' ') {
                        coordinates[ind][i*height+j] = 0; // TODO: -1 or 0?
                    }
                    
                }
            } // end single example    
        } // end all examples
    } // end file if
    trainLabel.close();
    trainImgs.close();
    
    // priors
//    printf("\npriors: \n");
//    for(int p = 0; p < 10; p++) {
//        double l = (double) trainExamplesCount[p] / (double) trainCount;
//        printf("%d: %f\n",p,l);
//    }    
    
//    printf("\nimg 0:\n");
//    vector<int> one = coordinates[0];
//    for(int i = 0; i < 28; i++) {
//        for(int j = 0; j < 28; j++) {
//            printf(" %-5.1d,",one[i*28+j]);
//        }
//        cout << endl;
//    }
}

void knnSolver::test() {
    vector<int> testExamplesCount, testSuccesses;
    vector<int> testimgLabels;
    vector<vector<int>> imgvec; 
    int totalsuccess = 0;
    vector<double> confusion;
    
    ifstream testImgs, testLabel;
    
    imgvec.resize(testCount);
    for(int r = 0; r < testCount; r++) {
        imgvec[r].resize(featureCount, -1);
    }
    
    testSuccesses.resize(classCount);
    confusion.resize(classCount*classCount);
    testExamplesCount.resize(classCount, 0);
    testimgLabels.resize(testCount, -1);
    testImgs.open(testImages);
    testLabel.open(testLabels);

    
    //read values into imgvec
    int label = 0, ind = 0;
    if(testLabel.is_open() && testImgs.is_open()) {
        for(ind; ind < testCount; ind++) {
            char val;            
            
            testLabel >> label;
            testExamplesCount[label] += 1; // count occurrences
            testimgLabels[ind] = label;
            
            for(int i = 0; i < height; i++) {
                for(int j = 0; j <= width; j++) {
                    testImgs.get(val);
                    if(val == '#' || val == '+') {
                        imgvec[ind][i*height+j] = 1;
                    } else if(val == ' ') {
                        imgvec[ind][i*height+j] = 0; // TODO: -1 or 0?
                    }
                    
                }
            } // end single example    
        } // end all examples
    } // end file if
    testLabel.close();
    testImgs.close();
    
    
    // test each entry
    for(int testi = 0; testi < testCount; testi++) { // testing data TODO testCount
        printf("testind: %d\n",testi);
        vector<int> neighborCount;
        vector<int> neighborClass;
        vector<int> neighborDist;
        int correctClass = testimgLabels[testi];

        neighborClass.resize(0);
        neighborDist.resize(0);
        neighborCount.resize(classCount, 0);
        
        // find all distances
        int toBeat = -9;
        for(int traini = 0; traini < trainCount; traini++) {
            int dist = distance(coordinates[traini], imgvec[testi]);
//            printf("test: %d comp: %d dist: %f",correctClass, trainLabels[traini],dist);
            if(neighborClass.size() < k) { // add first k anyways
//                printf("ADD test: %d comp: %d dist: %f size: %d, k: %d\n",correctClass, trainLabels[traini],dist,neighborClass.size(),k);
                neighborClass.push_back(trainLabels[traini]);
                neighborDist.push_back(dist);
                if(neighborClass.size() == k) { // set to beat
                    for(int nn = 0; nn < k; nn++) {
                        if(neighborDist[nn] > toBeat) toBeat = neighborDist[nn];
                    }
                }
            } else {
                if(dist < toBeat) {
//                    printf("test: %d comp: %d dist: %d toBeat: %d\n",correctClass, trainLabels[traini],dist,toBeat);
                    // find farthest neighbor and replace
                    for(int nn = 0; nn < k; nn++) {
                        if(neighborDist[nn] == toBeat) {
//                            printf("test: %d comp: %d dist: %d toBeat: %d\n",correctClass, trainLabels[traini],dist,toBeat);
                            neighborClass[nn] = trainLabels[traini];
                            neighborDist[nn] = dist;
                            for(int jj = 0; jj < neighborClass.size(); jj++) {
//                                printf("%d,%d ",neighborClass[jj],neighborDist[jj]);
                            }
                        }
                    }
                    // update toBeat
                    int localmx = -1;
                    for(int nn = 0; nn < k; nn++) {
                        if(neighborDist[nn] > localmx) localmx = neighborDist[nn];
                    }
                    toBeat = localmx;
                }
            }
        }// end training scan
        
        // count k nearest
        for(int kc = 0; kc < k; kc++) {
//            printf("found %d\n",neighborClass[kc]);
            neighborCount[neighborClass[kc]]++;
        }
        
        // return max nearest
        int maxC = -1;
        int maxNI = -1;
        for(int ni = 0; ni < classCount; ni++) { // compare against each class weight
            int count = neighborCount[ni];
            if(count > maxC) {
                maxC = count;
                maxNI = ni;
            }
        } // end weight comparison
        
        // count successes
        confusion[correctClass*classCount+maxNI] += 1.0;
        if(correctClass == maxNI) { //update for wrong guess
            testSuccesses[correctClass]++;
            totalsuccess++;
        } else {
            // do nothing???
        }
    } // end one set of test data
    
    
    // accuracy and such
    printf("\nIndividual success rates:\n");
    for(int s = 0; s < classCount; s++) {
        double successRate = (double) testSuccesses[s] / (double) testExamplesCount[s];
        printf("%d: %f\n",s,successRate);
    }
    double totalRate = (double) totalsuccess / (double)testCount;
    printf("total success rate: %f\n", totalRate);
    
    // show confusion matrix
    printf("\nConfusion Matrix:\n");
    printf("  %6d%6d%6d%6d%6d%6d%6d%6d%6d%6d\n",0,1,2,3,4,5,6,7,8,9);
    for(int r = 0; r < classCount; r++) {
        printf("%d  ",r);
        for(int c = 0; c < classCount; c++) {
            confusion[r*classCount+c] = 100*(confusion[r*classCount+c]/ (double) testExamplesCount[r]);
            printf("%5.2f ",confusion[r*classCount+c]);
        }
        printf("\n");
    }
}

void knnSolver::solve() {
    
    load();
    test();

}

knnSolver::knnSolver() {
}

knnSolver::knnSolver(string testImages, string testLabels, string trainingImages, string trainingLabels, int k) {
    this->testImages = testImages;
    this->testLabels = testLabels;
    this->trainingImages = trainingImages;
    this->trainingLabels = trainingLabels;
    this->k = k;
    
    trainLabels.resize(trainCount, -1);
    
    coordinates.resize(trainCount);
    for(int r = 0; r < trainCount; r++) {
        coordinates[r].resize(featureCount, -1);
    }
    
}

knnSolver::knnSolver(const knnSolver& orig) {
}

knnSolver::~knnSolver() {
}




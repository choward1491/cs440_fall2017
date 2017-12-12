
/* 
 * File:   digitSolver.cpp
 * Author: lukep
 * 
 * Created on November 27, 2017, 5:29 PM
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <ctime>
#include <random>
#include <algorithm>
#include "digitSolver.h"

#define featureCount 784
#define testCount 1000
#define trainCount 5000
#define classCount 10
#define width 28
#define height 28

using namespace std;

double digitSolver::dotProduct(const vector<double>& a, const vector<double>& b) {
    double ret = 0.0;
    for(int i = 0; i < featureCount; i++) {
        ret += a[i] * b[i];
    }
    return ret;
}

void digitSolver::updateWeight(int wi, vector<double> img, double alpha, bool add) {
    int sign = (add) ? 1 : -1;
//    printf("alpha: %f\n",alpha);
    for(int i = 0; i < featureCount; i++) {
        weights[wi][i] += (sign)*(alpha*img[i]);
    }
}
 

// train the weights in epochs
void digitSolver::setWeights() {
    double alpha = 1.0;
    vector<int> trainExamplesCount;
    vector<int> trainLabels;
    vector<vector<double>> imgvec; 
    
    
    ifstream trainImgs, trainLabel;
    
    imgvec.resize(trainCount);
    for(int r = 0; r < trainCount; r++) {
        imgvec[r].resize(featureCount, -9.0);
    }
    
    trainExamplesCount.resize(classCount, 0);
    trainLabels.resize(trainCount, -1);
    trainImgs.open(trainingImages);
    trainLabel.open(trainingLabels);

    
    //read values into imgvec
    int label = 0, ind = 0;
    if(trainLabel.is_open() && trainImgs.is_open()) {
        for(ind; ind < trainCount; ind++) {
            char val;            
            
            trainLabel >> label;
            trainExamplesCount[label] += 1; // count occurrences
            trainLabels[ind] = label;
//            printf("class: %d:\n",label);
            
            for(int i = 0; i < height; i++) {
                for(int j = 0; j <= width; j++) {
                    trainImgs.get(val);
                    if(val == '#' || val == '+') {
                        imgvec[ind][i*height+j] = 1.0;
//                        printf("%1.1f,",imgvec[ind][i*height+j]);
                    } else if(val == ' ') {
                        imgvec[ind][i*height+j] = 0.0; // TODO: -1 or 0?
//                        printf("%1.1f,",imgvec[ind][i*height+j]);
                    }
                    
                }
//                std::cout << endl;
            } // end single example    
        } // end all examples
    } // end file if
    trainLabel.close();
    trainImgs.close();
    
    
    // train weights
    for(int e = 0; e < setepochs; e++) { // epochs
        if(setrandomorder) {
            auto seed = unsigned ( std::time(0) );
            std::srand ( seed );
            std::random_shuffle ( imgvec.begin(), imgvec.end() );
            std::srand ( seed );
            std::random_shuffle ( trainLabels.begin(), trainLabels.end() );
        }
        for(int tr = 0; tr < trainCount; tr++) { // training data
            double maxdot = -999;
            int maxclass = -1;
            int correctClass = trainLabels[tr];
            
            for(int wv = 0; wv < classCount; wv++) { // compare against each class weight
                double dr = dotProduct(imgvec[tr], weights[wv]);
                if(setbias) dr += biases[wv];
                if(dr > maxdot) {
                    maxdot = dr;
                    maxclass = wv;
                }
            } // end weight comparison
            if(correctClass != maxclass) { //update for wrong guess
                updateWeight(correctClass, imgvec[tr], alpha, true);
                updateWeight(maxclass, imgvec[tr], alpha, false);
                if(setbias) {
                    biases[correctClass] += alpha*1.0;
                    biases[maxclass] -= alpha*1.0;
                }
            } else {
                // do nothing???
            }
        } // end one set of train data
        alpha = (double)setepochs/((double)setepochs + (double)e); //update learning rate
//        alpha = pow(2.718,(double)e*(-1.0/(double)setepochs)); //update learning rate
//        alpha = (-1.0/(double)setepochs)*(double)e + 1.0;
    } // end all epochs
    
    // priors
    printf("\npriors: \n");
    for(int p = 0; p < 10; p++) {
        double l = (double) trainExamplesCount[p] / (double) trainCount;
        printf("%d: %f\n",p,l);
    }
    
   // biases
    printf("\nbiases: \n");
    for(int b = 0; b < 10; b++) {
        printf("%d: %f\n",b,biases[b]);
    }
    
    
//    printf("\nweights 0:\n");
//    vector<double> one = weights[0];
//    for(int i = 0; i < 28; i++) {
//        for(int j = 0; j < 28; j++) {
//            printf(" %-5.1f,",one[i*28+j]);
//        }
//        cout << endl;
//    }
}

void digitSolver::test() {
    vector<int> testExamplesCount, testSuccesses;
    vector<int> testimgLabels;
    vector<vector<double>> imgvec; 
    int totalsuccess = 0;
    vector<double> confusion;
    
    ifstream testImgs, testLabel;
    
    imgvec.resize(testCount);
    for(int r = 0; r < testCount; r++) {
        imgvec[r].resize(featureCount, -9.0);
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
//            printf("class: %d:\n",label);
            
            for(int i = 0; i < height; i++) {
                for(int j = 0; j <= width; j++) {
                    testImgs.get(val);
                    if(val == '#' || val == '+') {
                        imgvec[ind][i*height+j] = 1.0;
//                        printf("%1.1f,",imgvec[ind][i*height+j]);
                    } else if(val == ' ') {
                        imgvec[ind][i*height+j] = 0.0; // TODO: -1 or 0?
//                        printf("%1.1f,",imgvec[ind][i*height+j]);
                    }
                    
                }
//                std::cout << endl;
            } // end single example    
        } // end all examples
    } // end file if
    testLabel.close();
    testImgs.close();
    
    // test each entry
    for(int tr = 0; tr < testCount; tr++) { // testing data
        double maxdot = -999;
        int maxclass = -1;
        int correctClass = testimgLabels[tr];

        for(int wv = 0; wv < classCount; wv++) { // compare against each class weight
            double dr = dotProduct(imgvec[tr], weights[wv]);
            if(setbias) dr += biases[wv];
            if(dr > maxdot) {
                maxdot = dr;
                maxclass = wv;
            }
        } // end weight comparison
        confusion[correctClass*classCount+maxclass] += 1.0;
        if(correctClass == maxclass) { //update for wrong guess
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

void digitSolver::solve() {
    
    setWeights();
    test();

}

digitSolver::digitSolver() {
}

digitSolver::digitSolver(string testImages, string testLabels, string trainingImages, string trainingLabels, 
        double setdecay, bool setbias, bool setinitalzero, bool setrandomorder, int setepochs) {
    this->testImages = testImages;
    this->testLabels = testLabels;
    this->trainingImages = trainingImages;
    this->trainingLabels = trainingLabels;
    this->setdecay = setdecay;
    this->setbias = setbias;
    this->setinitalzero = setinitalzero;
    this->setrandomorder = setrandomorder;
    this->setepochs = setepochs;
    
    biases.resize(classCount, 0.0);
    
    weights.resize(classCount);
    for(int r = 0; r < classCount; r++) {
        weights[r].resize(featureCount, 0.0);
    }
    
    if(!setinitalzero) {
        for(int r = 0; r < classCount; r++) {
            for(int s = 0; s < featureCount; s++) {
                weights[r][s] = (double)(rand()%10);
            }
        }
    }
}

digitSolver::digitSolver(const digitSolver& orig) {
}

digitSolver::~digitSolver() {
//    printf("destruct\n");
}


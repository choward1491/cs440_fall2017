# This script is to handle work being done for MP3 Problem 1.1
# Author: Christian Howard

# import useful libraries
import numpy                as np
import matplotlib.pyplot    as plot
import myml.images          as myimg
import mp3.load_data        as myload
import mp3.discriminant     as mydisc
from timeit import default_timer as timer

# index for independent var data and labels
vars = 0
lbls = 1

# define classes
classes = np.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])

# load the dataset in a proper form
(Test,Train) = myload.loadProblem1Data('data/prob1/part1')

# start timer to check for training runtime
start = timer()

# create the discriminant functions
possible_values = np.array([0,1])
set = mydisc.constructDistriminantSet(X=Train[vars],
                                      L=Train[lbls],
                                      possible_values=possible_values,
                                      laplace_smoothing_val=0.1)

# find time the training finished
end = timer()
print('Training took {0} seconds'.format(end - start))

# start timer to check for training runtime
start = timer()
# evaluate the discriminant functions
Labels = mydisc.evalDiscriminantSet(Test[vars],set)
# find time the training finished
end = timer()
print('Testing took {0} seconds'.format(end - start))

# compute the confusion matrix along with accuracy
(C,accuracy) = mydisc.computeConfusionMatrix(Test[vars],Test[lbls],Labels,classes)
print('The overall accuracy is {0}%'.format(accuracy))

# plot the confusion matrix
plotdir = 'plots/prob1'
f0 = plot.figure()
(f0,ax0) = myimg.plotDataset(f0,C,delta=(-1,-1))
ax0.set_xlabel('Classified Classes')
ax0.set_ylabel('True Classes')
f0.savefig('{0}/confusion_mat.png'.format(plotdir))

# print classification rates for each number
for i in range(0,classes.shape[0]):
    print('Accuracy for digit {0} is {1}'.format(i,C[i,i]))

# Find the largest confusion values in the matrix and get their tuple pair
cbest = mydisc.computeMostConfused(C,num_values=4)

# For each pair of confused values, generate and save each digit's likelihood and the
# pair's log odds ratio
for (i1, i2, cval) in cbest:
    fig = plot.figure()
    l1 = set[i1].likelihood(class_value=1).reshape(28,28)
    l2 = set[i2].likelihood(class_value=1).reshape(28,28)
    odds = l1 - l2

    # likelihood of digit i1
    (f1,ax1) = myimg.plotDataset(fig,l1,doFlip=False)
    ax1.set_xlabel('Width')
    ax1.set_ylabel('Height')
    f1.savefig('{}/llhd{:.0f}.png'.format(plotdir, i1))

    # likelihood of digit i2
    fig.clear()
    (f2,ax2) = myimg.plotDataset(fig,l2,doFlip=False)
    ax2.set_xlabel('Width')
    ax2.set_ylabel('Height')
    f2.savefig('{}/llhd{:.0f}.png'.format(plotdir, i2))

    # odds ratio
    fig.clear()
    (f3, ax3) = myimg.plotDataset(fig, odds,doFlip=False)
    ax3.set_xlabel('Width')
    ax3.set_ylabel('Height')
    f3.savefig('{}/odds_{:.0f}_{:.0f}.png'.format(plotdir, i1, i2))

# compute the testing samples that have the highest and lowest posterior probabilities
for class_ in classes:
    fig = plot.figure()
    (Xmin,Xmax) = mydisc.computeMaxMinPosteriorData(set, Test[vars], Test[lbls], class_)
    Xmin = Xmin.reshape(28,28)
    Xmax = Xmax.reshape(28,28)

    (f1,ax1) = myimg.plotDataset(fig,Xmin,doFlip=False)
    ax1.set_xlabel('Width')
    ax1.set_ylabel('Height')
    f1.savefig('{}/class{}_minPosteriorTestData.png'.format(plotdir, class_))

    fig.clear()
    (f2,ax2) = myimg.plotDataset(fig,Xmax,doFlip=False)
    ax2.set_xlabel('Width')
    ax2.set_ylabel('Height')
    f2.savefig('{}/class{}_maxPosteriorTestData.png'.format(plotdir, class_))




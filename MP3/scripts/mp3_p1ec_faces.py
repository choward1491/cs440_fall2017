# This script is to handle work being done for MP3 Problem 1 Face Extra Credit
# Author: Christian Howard

# import useful libraries
import numpy                as np
import matplotlib.pyplot    as plot
import myml.images          as myimg
import mp3.load_data        as myload
import mp3.discriminant     as mydisc
import mp3.features         as myfeat
from timeit import default_timer as timer

# index for independent var data and labels
vars = 0
lbls = 1

# define classes
classes = np.array([0, 1])

def feature_functor(X, L):
    return myfeat.disjoint_patches(X, L, dims=(2,2), nvals=2)

# load the dataset in a proper form
(Test,Train) = myload.loadProblem1Faces('data/prob1/face',functor=feature_functor)

# start timer to check for training runtime
start = timer()

# create the discriminant functions
possible_values = np.arange(0,2**4-1,1,dtype=int)
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
plotdir = 'plots/prob1_face'
f0 = plot.figure()
(f0,ax0) = myimg.plotDataset(f0,C,delta=(-1,-1))
ax0.set_xlabel('Classified Classes')
ax0.set_ylabel('True Classes')
f0.savefig('{0}/confusion_mat.png'.format(plotdir))

# Find the largest confusion values in the matrix and get their tuple pair
cbest = mydisc.computeMostConfused(C,num_values=4)

# For each pair of confused values, generate and save each digit's likelihood and the
# pair's log odds ratio
for (i1, i2, cval) in cbest:
    fig = plot.figure()
    l1 = set[i1].likelihood(class_value=1).reshape(35,30)
    l2 = set[i2].likelihood(class_value=1).reshape(35,30)
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

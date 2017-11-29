# This script is to handle work being done for MP3 Problem 1.1 for overlapping features
# Author: Christian Howard

# import useful libraries
import numpy                as np
import matplotlib.pyplot    as plot
import myml.images          as myimg
import mp3.load_data        as myload
import mp3.discriminant     as mydisc
import mp3.features         as myfeat
import os, errno
from timeit import default_timer as timer

# index for independent var data and labels
vars = 0
lbls = 1

# define classes
classes = np.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])

# define patch dimension list
dims = [(2,2),(2,4),(4,2),(4,4),(2,3),(3,2),(3,3)]

# loop through the different patch dimensions
for dim in dims:

    # define functor to generate data with appropriate features
    def feature_functor(X,L):
        return myfeat.overlapping_patches(X,L,dims=dim,nvals=2)

    # load the dataset in a proper form
    (Test,Train) = myload.loadProblem1Data('data/prob1/part1',functor=feature_functor)

    # start timer to check for training runtime
    start = timer()

    # create the discriminant functions
    possible_values = np.arange(0,2**(dim[0]*dim[1]),dtype=int)
    set = mydisc.constructDistriminantSet(X=Train[vars],L=Train[lbls],possible_values=possible_values,laplace_smoothing_val=0.1)

    # find time the training finished
    end = timer()
    print('({0},{1})| Training took {2} seconds'.format(dim[0],dim[1],end - start))

    # start timer to check for training runtime
    start = timer()
    # evaluate the discriminant functions
    Labels = mydisc.evalDiscriminantSet(Test[vars], set)
    # find time the training finished
    end = timer()
    print('({0},{1})| Testing took {2} seconds'.format(dim[0], dim[1], end - start))

    # compute the confusion matrix along with accuracy
    (C,accuracy) = mydisc.computeConfusionMatrix(Test[vars],Test[lbls],Labels,classes)
    print('({0},{1})| The overall accuracy is {2}%'.format(dim[0],dim[1],accuracy))

    # generate appropriate paths
    plotdir0    = 'plots/prob12_overlap'
    plotdir     = 'plots/prob12_overlap/{0}_{1}'.format(dim[0],dim[1])
    try:
        os.makedirs(plotdir0)
    except OSError as e:
        if e.errno != errno.EEXIST:
            raise

    try:
        os.makedirs(plotdir)
    except OSError as e:
        if e.errno != errno.EEXIST:
            raise

    # plot the confusion matrix
    f0 = plot.figure()
    (f0,ax0) = myimg.plotDataset(f0,C,delta=(-1,-1))
    ax0.set_xlabel('Classified Classes')
    ax0.set_ylabel('True Classes')
    f0.savefig('{0}/confusion_mat.png'.format(plotdir))
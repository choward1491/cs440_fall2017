# This script is to handle work being done for MP3 Problem 2.1
# Author: Christian Howard

# import useful libraries
import numpy                as np
import matplotlib.pyplot    as plot
import myml.images          as myimg
import mp3.load_data        as myload
import mp3.discriminant     as mydisc

# index for independent var data and labels
vars = 0
lbls = 1

# define classes
classes = np.array([0, 1])

# load the dataset in a proper form
(Test,Train) = myload.loadProblem2Part1('data/prob2/part1')

# create the discriminant functions
possible_values = np.array([0,1])
set = mydisc.constructDistriminantSet(X=Train[vars],
                                      L=Train[lbls],
                                      possible_values=possible_values,
                                      laplace_smoothing_val=1e-1)

# evaluate the discriminant functions
Labels = mydisc.evalDiscriminantSet(Test[vars],set)

# compute the confusion matrix along with accuracy
(C,accuracy) = mydisc.computeConfusionMatrix(Test[vars],Test[lbls],Labels,classes)
print('The overall accuracy is {0}%'.format(accuracy))

# plot the confusion matrix
plotdir = 'plots/prob21'
f0 = plot.figure()
(f0,ax0) = myimg.plotDataset(f0,C,delta=(-1,-1))
ax0.set_xlabel('Classified Classes')
ax0.set_ylabel('True Classes')
f0.savefig('{0}/confusion_mat.png'.format(plotdir))



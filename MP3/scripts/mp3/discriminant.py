#import important libraries
import numpy as np


def computeConfusionMatrix( Xtest, Ltest, Leval, classes ):

    # init useful vars
    Nc  = classes.shape[0]
    C   = np.zeros((Nc,Nc))

    # compute the confusion matrix
    for ir in range(0,Nc):
        ridx = np.where(Ltest==classes[ir])[0]
        nr = ridx.shape[0]
        for ic in range(0,Nc):
            cidx = np.where(Leval[ridx] == classes[ic])[0]
            nc = cidx.shape[0]
            C[ir,ic] = (100.0*nc)/(nr)

    # compute the accuracy
    dL = Ltest - Leval
    N = dL.shape[0]
    bidx = np.where(dL == 0)[0]
    accuracy = (100.0 * bidx.shape[0]) / N

    # return confusion matrix and accuracy
    return (C,accuracy)


def computeMaxMinPosteriorData( discriminant_list, Xtest, Ltest, dclass ):

    # get indices for data corresponding to class `dclass`
    ridx = np.where(Ltest == dclass)[0]

    # evaluate the discriminant function tied to `dclass`
    # on the `dclass` dataset
    results = discriminant_list[dclass].eval(Xtest[:,ridx])

    # get the indices for the data with the largest
    # and the lowest posterior probability
    max_idx = np.argmax(results, axis=0)
    min_idx = np.argmin(results, axis=0)

    # return the data associated with the lowest and highest
    # posterior probabilities
    return (Xtest[:,ridx[min_idx]], Xtest[:,ridx[max_idx]])


def computeMostConfused(C, num_values = 4):

    # define function to sort the data
    def sort_func(data):
        if data[0] != data[1]:
            return -data[2]
        else:
            return np.log(1e8)

    # get the number of elements in C
    # and allocate storage for row and column matrices
    nt   = C.shape[0]*C.shape[1]
    rows = np.zeros(C.shape)
    cols = np.zeros(C.shape)
    for r in range(0,C.shape[0]):
        for c in range(0,C.shape[1]):
            rows[r,c] = r
            cols[r,c] = c

    # sort row and columns such that we can get a list
    # of the most confused pairs
    rt    = rows.reshape((nt,))
    ct    = cols.reshape((nt,))
    Ct    = C.reshape((nt,))
    clist   = list(zip(rt,ct,Ct))
    cnew    = sorted(clist,key=sort_func)

    # return top N confused pairs based on
    # values in confusion matrix
    return cnew[:num_values]

def constructDistriminantSet(X, L, possible_values, laplace_smoothing_val = 1.0):

    # get shape of data and find unique labels
    (d,nd) = X.shape
    u = np.unique(L)
    Nu = u.shape[0]
    set = dict()

    # construct set of discriminant functions based on the dataset,
    # the smoothing constant, and the possible values a feature can take
    for n in range(0,Nu):
        idx = np.where(L == u[n])[0]
        Xt = X[:,idx[:]]
        set[u[n]] = Discriminant(Xt,nd, possible_values, laplace_smoothing_val)

    # return set of discriminant functions
    return set

def evalDiscriminantSet(X, discriminant_list, classes = None):
    # Author: Christian Howard
    # Function to compute the classification given some input X
    # and a list of discriminant functions that could label the
    # input data

    # get the total number of labels
    nlbl = len(discriminant_list)

    # get dimensions of data
    (d,nd) = X.shape

    # init matrix for evaluating discriminants against data
    results = np.zeros((nlbl, nd))

    # loop discriminant functions and figure out
    # the classification for each data point in X
    if classes is None:
        for i in range(0, nlbl):
            results[i, :] = discriminant_list[i].eval(X)
    else:
        for i in range(0, nlbl):
            results[i, :] = discriminant_list[classes[i]].eval(X)

    # for each data point, find the index of the discriminant
    # that says the data is best represented by its distribution
    max_idx = np.argmax(results, axis=0)

    # return the max_idx which represents
    # the classification for each data point in X
    if classes is None:
        return max_idx
    else:
        return classes[max_idx]

class Discriminant:
    # Author: C. Howard
    # Class representing a naive bayes implementation that uses
    # basic frequency based distributions

    def __init__(self):
        self.pv             = []
        self.Pomega         = 0.0
        self.table          = []
        self.cost_diff      = 1.0
        self.laplacesmooth  = 1.0

    def __init__(self, lbl_dataset, num_total_data, possible_values, k=1.0, cost_diff=1.0):
        (d, nd) = lbl_dataset.shape
        self.laplacesmooth  = k
        self.Pomega         = nd / num_total_data
        self.pv             = possible_values
        self._setTable(lbl_dataset,possible_values)
        self.cost_diff      = cost_diff

    def _setTable(self, lbl_dataset, possible_values, k=1.0):
        # this method essentially trains the classifier by building
        # a laplace-smoothed table of probabilities
        (d, nd)     = lbl_dataset.shape
        (nv,)       = possible_values.shape
        self.table  = np.zeros((d, nv))

        # loop through features and their possible values
        # to compute the probability of them appearing
        for dim in range(0,d):
            for idx in range(0,nv):
                matches = np.count_nonzero( lbl_dataset[dim,:].reshape(1,nd) == possible_values[idx])
                self.table[dim,idx] = (matches + k)/(nd + k*nv)

    def likelihood(self, class_value):

        # compute the log likelihood of some feature value existing
        nf = self.table.shape[0]
        llhd = np.zeros((nf,1))

        # loop through feaures and compute log likelihood
        # of each feature being some value
        for n in range(0,nf):
            llhd[n] = np.log(self.table[n,class_value])

        # return log likelihood
        return llhd

    def eval(self,x):

        # evaluate the discriminant function at some chunk of data x
        (d, nd) = x.shape
        value = np.zeros((nd,)) + np.log(self.cost_diff) + np.log(self.Pomega)

        # loop through data and each feature element
        # and use to compute probability value for each
        # input data vector
        for n in range(0,nd):
            for dim in range(0, d):
                idx = int(x[dim,n])
                value[n] += np.log( self.table[dim,idx] )

        # return output probability result
        return value
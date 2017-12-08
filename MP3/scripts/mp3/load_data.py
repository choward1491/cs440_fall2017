# import useful libraries
import numpy as np
import mp3.features as myfeatures


def parseLabelString(ldata):
    # Author: C. Howard
    # Function to take string of label data and
    # convert it into an array of label data
    nc = len(ldata)
    numbers = ldata.split('\n')
    nn = len(numbers)
    if numbers[nn-1] == '': nn -= 1
    labels = np.zeros((nn,))

    # extract labels from list of label strings
    for n in range(0,nn):
        labels[n] = int(numbers[n])

    # return the labels
    return labels

def parseImageString(sdata, img_dims, newline_space = 0):
    # Author: C. Howard
    # Function to take input string of image data, sdata,
    # and the image dimensions for each image, img_dims,
    # and converts all of this into a list of images
    nc = len(sdata)
    ni = int(nc/((img_dims[0])*(img_dims[1]+1) + newline_space))
    nf = img_dims[0]*img_dims[1]
    images = np.zeros((ni,),dtype=object)
    ncols = img_dims[1]+1

    # loop through string data and build images
    # as a list of matrices
    offset = 0
    for n in range(0,ni):
        images[n] = np.zeros(img_dims)
        for r in range(0,img_dims[0]):
            for c in range(0,img_dims[1]+1):
                char = sdata[offset + c + r*ncols]
                if char == '+' or char == '#' or char == '%':
                    images[n][r,c] = 1.0
        offset += (img_dims[0])*(img_dims[1]+1) + newline_space


    # return the images
    return images


def loadProblem1Data(directory_path, functor = myfeatures.vectorize):
    # Author: C. Howard
    # Purpose of this code is to load the data for Part 1
    # of MP3.
    # directory_path:   This is the path to the directory storing the test data
    # functor:          This input operates on the list of images to convert them
    #                   into whatever feature-based dataset the functor desires

    # modify path if needed to include forward slash
    np = len(directory_path)
    if directory_path[np-1] != '/':
        directory_path += "/"

    # specify dimensions of images
    idims = (28,28)

    # get testing data
    (Xtest,Ltest) = functor(parseImageString(open("{0}testimages".format(directory_path),"r").read(), idims),
                            parseLabelString(open("{0}testlabels".format(directory_path), "r").read()))

    # get training data
    (Xtrain,Ltrain) = functor(parseImageString(open("{0}trainingimages".format(directory_path), "r").read(), idims),
                              parseLabelString(open("{0}traininglabels".format(directory_path), "r").read()))

    # return the datasets as tuples
    return ((Xtest,Ltest), (Xtrain,Ltrain))


def loadProblem1Faces(directory_path, functor = myfeatures.vectorize):
    # Author: C. Howard
    # Purpose of this code is to load the face data for Part 1 EC
    # of MP3.
    # directory_path:   This is the path to the directory storing the test data
    # functor:          This input operates on the list of images to convert them
    #                   into whatever feature-based dataset the functor desires

    # modify path if needed to include forward slash
    np = len(directory_path)
    if directory_path[np-1] != '/':
        directory_path += "/"

    # specify dimensions of images
    idims = (70,60)

    # get testing data
    (Xtest,Ltest) = functor(parseImageString(open("{0}facedatatest".format(directory_path),"r").read(), idims),
                            parseLabelString(open("{0}facedatatestlabels".format(directory_path), "r").read()))

    # get training data
    (Xtrain,Ltrain) = functor(parseImageString(open("{0}facedatatrain".format(directory_path), "r").read(), idims),
                              parseLabelString(open("{0}facedatatrainlabels".format(directory_path), "r").read()))

    # return the datasets as tuples
    return ((Xtest,Ltest), (Xtrain,Ltrain))


def loadProblem2Part1(directory_path, functor = myfeatures.vectorize):
    # Author: C. Howard
    # Purpose of this code is to load the data for Problem 2.1
    # of MP3.
    # directory_path:   This is the path to the directory storing the test data
    # functor:          This input operates on the list of images to convert them
    #                   into whatever feature-based dataset the functor desires

    # modify path if needed to include forward slash
    ndp = len(directory_path)
    if directory_path[ndp - 1] != '/':
        directory_path += "/"

    # dimensions for data
    idims = (25,10)

    # get testing data
    NoTest  = parseImageString(open("{0}no_test.txt".format(directory_path), "r").read(),idims,newline_space=3)
    YesTest = parseImageString(open("{0}yes_test.txt".format(directory_path), "r").read(),idims,newline_space=3)
    NetTest = np.concatenate( (NoTest,YesTest) )
    (Xtest, Ltest) = functor(NetTest, np.concatenate(
                                                (np.zeros(NoTest.shape,dtype=int),
                                                 np.ones(YesTest.shape,dtype=int)
                                                 )))

    # get training data
    NoTrain     = parseImageString(open("{0}no_train.txt".format(directory_path), "r").read(),idims,newline_space=3)
    YesTrain    = parseImageString(open("{0}yes_train.txt".format(directory_path), "r").read(),idims,newline_space=3)
    NetTrain    = np.concatenate((NoTrain, YesTrain))
    (Xtrain, Ltrain) = functor(NetTrain, np.concatenate(
                                                (np.zeros(NoTrain.shape,dtype=int),
                                                 np.ones(YesTrain.shape,dtype=int)
                                                 )))

    # return the datasets as tuples
    return ((Xtest, Ltest), (Xtrain, Ltrain))


def loadProblem2Part2(directory_path, functor = myfeatures.vectorize):
    # Author: C. Howard
    # Purpose of this code is to load the data for Problem 2.2
    # of MP3.
    # directory_path:   This is the path to the directory storing the test data
    # functor:          This input operates on the list of images to convert them
    #                   into whatever feature-based dataset the functor desires

    # modify path if needed to include forward slash
    ndp = len(directory_path)
    if directory_path[ndp - 1] != '/':
        directory_path += "/"

    # specify dimensions of data "images"
    idims = (30, 13)

    # get testing data
    (Xtest, Ltest) = functor(parseImageString(open("{0}testing_data.txt".format(directory_path), "r").read(),
                                              idims,
                                              newline_space=3),
                             parseLabelString(open("{0}testing_labels.txt".format(directory_path), "r").read()))

    # get training data
    (Xtrain, Ltrain) = functor(
        parseImageString(open("{0}training_data.txt".format(directory_path), "r").read(),
                         idims,
                         newline_space=3),
        parseLabelString(open("{0}training_labels.txt".format(directory_path), "r").read()))

    # return the datasets as tuples
    return ((Xtest, Ltest), (Xtrain, Ltrain))
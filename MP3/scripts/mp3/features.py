import numpy as np

def vectorize(X, L):
    # Author: C. Howard
    # method to vectorize a given list of order 2 tensors
    nx = X.shape[0]
    (nr,nc) = X[0].shape
    Xn = np.zeros((nr*nc,nx))

    for n in range(0,nx):
        Xt      = X[n]
        Xn[:,n] = Xt[:,:].reshape(nr*nc,)

    return (Xn,L)

def disjoint_patches(X, L, dims, nvals = 2):
    # Author: C. Howard
    # Function for generating disjoint patch features
    nx = X.shape[0]
    (nr, nc) = X[0].shape
    grid = ( int(nr/dims[0]), int(nc/dims[1]) )
    nf = grid[0]*grid[1]
    Xn = np.zeros((nf, nx))

    # loop through the n images
    for n in range(0,nx):

        # loop through the patches for the nth image
        for ir in range(0, grid[0]):
            for ic in range(0, grid[1]):
                k = ir + grid[0]*ic
                factor = 1

                # loop through the pixels within the patches
                # so we can compute feature value for this patch
                for lr in range(0,dims[0]):
                    r = dims[0]*ir + lr
                    for lc in range(0,dims[1]):
                        c = dims[1]*ic + lc
                        Xn[k, n]    += factor*X[n][r,c]
                        factor      *= nvals

    # return the resulting dataset
    return (Xn,L)

def overlapping_patches(X, L, dims, nvals = 2):
    # Author: C. Howard
    # Function for generating overlapping patch features
    nx = X.shape[0]
    (nr, nc) = X[0].shape
    grid = ( int(nr - dims[0] + 1), int(nc - dims[1] + 1) )
    nf = grid[0]*grid[1]
    Xn = np.zeros((nf, nx))

    # loop through the n images
    for n in range(0,nx):

        # loop through the patches for the nth image
        for ir in range(0, grid[0]):
            for ic in range(0, grid[1]):
                k = ir + grid[0]*ic
                factor = 1

                # loop through the pixels within the patches
                # so we can compute feature value for this patch
                for lr in range(0,dims[0]):
                    r = ir + lr
                    for lc in range(0,dims[1]):
                        c = ic + lc
                        Xn[k, n]    += factor*X[n][r,c]
                        factor      *= nvals

    # return the resulting dataset
    return (Xn,L)

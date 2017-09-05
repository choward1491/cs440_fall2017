//
//  maze.cpp
//  MP1
//
//  Created by Christian J Howard on 9/5/17.
//  Copyright © 2017 C. Howard. All rights reserved.
//

#include "maze.hpp"

unsigned int maze::hash(unsigned int row, unsigned int col, unsigned int num_cols){
    return col + row*num_cols;
}

void maze::getRowColFromHash( unsigned int hash, unsigned int num_cols, int & row, int & col ){
    col = static_cast<int>(hash%num_cols);
    row = static_cast<int>(hash/num_cols);
}

//
// Created by Christian J Howard on 10/21/17.
//

#ifndef SRC_BREAKTHROUGH_ACTIONS_HPP
#define SRC_BREAKTHROUGH_ACTIONS_HPP

#include <stdint.h>

namespace bt {

    enum action_t: int {Forward = 0, LeftDiagonal, RightDiagonal};

    int actionHash( int k, action_t move, int numStateDims){
        return k + numStateDims*move;
    }
}

#endif //SRC_BREAKTHROUGH_ACTIONS_HPP

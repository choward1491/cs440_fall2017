//
// Created by Christian J Howard on 10/21/17.
//

#ifndef SRC_BREAKTHROUGH_STATE_HPP
#define SRC_BREAKTHROUGH_STATE_HPP

#include "breakthrough_piece_type.hpp"
#include <stdint.h>

namespace bt { // breakthrough namespace

    template<int NR = 8, int NC = 8>
    class state {
    public:

        // useful static funcs
        static int numStateDims() { return NR*NC; }
        static int numRows() { return NR; }
        static int numCols() { return NC; }
        static int hash(int row, int col){ return col + row*NC; }
        static std::pair<int,int> invhash(int k) { return std::pair<int,int>(k/NC, k%NC); };

        //setters/getters
        uint8_t getStateAt(int row, int col) const { return s[hash(row,col)]; }
        uint8_t getStateAt(int k) const { return s[k]; }
        void setStateAt(int row, int col, piece_t val ) { s[hash(row,col)] = static_cast<uint8_t>(val); }
        void setStateAt(int k, piece_t val ) { s[k] = static_cast<uint8_t>(val); }

        // operators
        bool operator==(const state & os ) const {
            bool isEqual = true;
            for (int i = 0; i < NR * NC; ++i) { isEqual &= (s[i] == os.s[i]); }
            return isEqual;
        }
        bool operator!=(const state & os) const { return !this->operator==(os); }

    private:
        uint8_t s[NR*NC];
    };
}

#endif //SRC_BREAKTHROUGH_STATE_HPP

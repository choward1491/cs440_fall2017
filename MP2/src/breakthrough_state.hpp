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

        void init(){
            for(int c = 0; c < NC; ++c){
                for(int r = 0; r < NR; ++r){
                    if( r <= 1 )        {   setStateAt(r,c,piece_t::Team1); }
                    else if(r >= NR-2 ) {   setStateAt(r,c,piece_t::Team2); }
                    else{                   setStateAt(r,c,piece_t::None);  }
                }// end for r
            }// end for c
        }

        //setters/getters
        uint32_t getNumPiecesCapturedBy( piece_t player ){
            const uint32_t initNumber = NC*2;
            uint32_t numPiecesLeft[2] = {0};
            for(int k = 0; k < NC*NR; ++k){
                if( s[k] == piece_t::Team1 ){
                    numPiecesLeft[piece_t::Team1]++;
                }else if( s[k] == piece_t::Team2 ){
                    numPiecesLeft[piece_t::Team2]++;
                }
            }// end for c

            return initNumber - numPiecesLeft[(player+1)%2];
        }
        uint8_t getStateAt(int row, int col) const { return s[hash(row,col)]; }
        uint8_t getStateAt(int k) const { return s[k]; }
        void setStateAt(int row, int col, int val ) { s[hash(row,col)] = static_cast<uint8_t>(val); }
        void setStateAt(int row, int col, piece_t val ) { s[hash(row,col)] = static_cast<uint8_t>(val); }
        void setStateAt(int k, piece_t val ) { s[k] = static_cast<uint8_t>(val); }

        // operators
        bool operator==(const state & os ) const {
            bool isEqual = true;
            for (int i = 0; i < NR * NC; ++i) { isEqual &= (s[i] == os.s[i]); }
            return isEqual;
        }
        bool operator!=(const state & os) const { return !this->operator==(os); }

        void print() const {
            for (int i = 0; i < NR; ++i) {
                printf("|");
                for (int j = 0; j < NC; ++j) {
                    uint8_t value = getStateAt(i, j);
                    switch (value) {
                        case 0: printf("X"); break;
                        case 1: printf("O"); break;
                        case 2:
                        default:printf(" "); break;
                    }
                }
                printf("|\n");
            }printf("\n");
        }

    private:
        uint8_t s[NR*NC];
    };
}

#endif //SRC_BREAKTHROUGH_STATE_HPP

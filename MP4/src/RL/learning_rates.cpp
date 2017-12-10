/*
 *  learning_rates.cpp
 *
 *  Project: MP4
 *  Author : Christian J Howard
 *  Date   : 12/7/17
 *  Purpose:
 *
 */

#include "learning_rates.hpp"

namespace RL {
    constant_rate::constant_rate(size_t ns, size_t na, double v):alpha(v){
        
    }
    double constant_rate::operator()(unsigned int iter, size_t s, size_t a) {
        return alpha;
    }
    
    inverse_decay_rate::inverse_decay_rate(size_t ns, size_t na,double v):C(v){
        
    }
    double inverse_decay_rate::operator()(unsigned int iter, size_t s, size_t a) {
        return C / (C + static_cast<double>(iter));
    }
    
    state_tracker_rate::state_tracker_rate(size_t ns_, size_t na_, double v):N(ns_*na_,0),C(v){
        ns = ns_; na = na_;
    }
    double state_tracker_rate::operator()(unsigned int iter, size_t s, size_t a) {
        N[s + ns*a]++;
        return C / (C + static_cast<double>(N[s + ns*a]) );
    }
}

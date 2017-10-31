
/* 
 * File:   flow_csp.h
 * Author: lukep
 * Copyright 2017 Luke Pitstick
 * Created on October 24, 2017, 1:35 AM
 */

#ifndef FLOW_CSP_H
#define FLOW_CSP_H

#include "csp_solver.h"
#include <set>
#include <vector>

namespace csp {
    namespace flow {

        class Domains {
        public:
            enum domain_type {A=0,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z};
            typedef std::map<size_t, std::set<enum domain_type>> var_domains;

            Domains(){};
            
            Domains(std::set<enum domain_type> colors){
//                std::set<enum domain_type> dset = colors;
//                std::set<enum domain_type> dset {A, B, C, D};
//                vd[0] = dset;
//                vd[1] = dset;
//                vd[2] = dset;
//                vd[3] = dset;
                vd[0] = colors;
            }

            var_domains vd;
        };

        class X {
        public:
            typedef enum Domains::domain_type value;
            X():visited(size(),false){}
            static size_t size() { return 4; }
            size_t getUnvisitedVariable() {
                for( size_t i = 0; i < size(); ++i){
                    if( !visited[i] ){ visited[i] = true; return i; }
                }
            }
        private:
            std::vector<bool> visited;
        };



        template<class Q>
        class Constraints {
            typedef std::map<size_t, typename Q::value> assignment;
            bool satisfyConstraints(size_t var, typename Q::value value, const assignment & assignment_){}
        };

        template<>
        class Constraints<X> {
        public:
            typedef std::map<size_t, typename X::value>                 assignment;
            typedef std::pair<size_t,X::value>                          pair_t;
            typedef std::map<size_t,bool (*)(X::value xi)>              unitary_constraints;
            typedef std::map<size_t,bool (*)(X::value xi, X::value xj)> binary_constraints;

            Constraints(){
                // set unitary constraints
                uc[0] = &Constraints::isA;

                // set binary constraints
                bc[hash(0,1)] = &Constraints::isNotSameValue;
                bc[hash(0,2)] = &Constraints::isNotSameValue;
                bc[hash(0,3)] = &Constraints::isNotSameValue;
                bc[hash(1,2)] = &Constraints::isNotSameValue;
                bc[hash(1,3)] = &Constraints::isNotSameValue;
                bc[hash(2,3)] = &Constraints::isNotSameValue;

            }
            bool satisfyConstraints(size_t var, typename X::value value, const assignment & assignment_){

                // check unitary constraints
                auto it = uc.find(var);
                if( it != uc.end() && !uc[var](value) ){ return false; }

                // check binary constraints
                for(auto it = assignment_.begin(); it != assignment_.end(); ++it ){
                    size_t min_ = 0, max_ = 0;
                    typename X::value minv, maxv;
                    if( var == it->first ){ continue; }
                    if( var < it->first )   {
                        min_ = var; max_ = it->first;
                        minv = value; maxv = it->second;
                    }
                    else{
                        min_ = it->first; max_ = var;
                        minv = it->second; maxv = value;
                    }

                    size_t hval = hash(min_,max_);
                    if( bc.find(hval) != bc.end() && !bc[hval](minv,maxv) ){
                        return false;
                    }
                }

                // return that constraints are satisfied if made it here
                return true;

            }

        private:

            unitary_constraints uc;
            binary_constraints  bc;

            static bool isA( X::value xi ){
                return xi == X::value::A;
            }

            static bool isNotSameValue( X::value xi, X::value xj){
                return xi != xj;
            }

            static size_t hash( size_t xi, size_t xj){
                return xi + X::size()*xj;
            }
        };

    }


    class flow_csp : public csp::solver<flow::X, flow::Domains, flow::Constraints, flow_csp> {
    public:
        typedef enum flow::Domains::domain_type domain_type;
        typedef flow::X::value val_type;
        typedef size_t var_type;
        
        size_t selectUnassignedVariable( csp_state & csp_ ){
            return csp_.x.getUnvisitedVariable();
        }

        const std::set<domain_type> & orderDomainValues(size_t var, const assignment & assignment_, const csp_state & csp_) {
            return csp_.d.vd.find(var)->second;
        }

        bool inference(size_t var, val_type value, csp_state & csp_, assignment & inference_list) {
            return false;
        }
    };

}

#endif /* FLOW_CSP_H */


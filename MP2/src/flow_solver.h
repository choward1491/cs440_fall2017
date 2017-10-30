
/* 
 * File:   flow_solver.h
 * Author: lukep
 *
 * Created on October 26, 2017, 9:32 PM
 */

#ifndef FLOW_SOLVER_H
#define FLOW_SOLVER_H

#include <set>
#include <vector>
#include <map>

class flow_solver {
public:
    enum domain_type {A=0,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,NONE};
    std::map<flow_solver::domain_type, char> dmap;
    
    void loadFlow(const std::string& flow_file);
    void saveFlow(const std::string & flow_file);
    void setSmart(bool beSmart);
    void printCurrent();
    void firstRestrict();
    int getUnvisitedVariable();
    int getAttempts();
    std::vector<int> getNeighbors(int var);
    std::vector<domain_type> getOrderedDomain(int ind);
    bool isConsistent(int var, domain_type val);
    bool checkNeighborOver(int var, domain_type val);
    bool sourceCheck(int var, domain_type val, int ignorevar);
    bool lastCheck();
    bool fullCheck();
    bool solve();
    
    flow_solver();
    flow_solver(const flow_solver& orig);
    virtual ~flow_solver();
    
    
//    typedef std::map<size_t, std::set<enum domain_type>> var_domains;
    
private:
//    var_domains vd;
    int ncol, nrow, attempts;
    bool beSmart;
    std::vector<bool> assigned;
    std::vector<bool> isSource;
    std::vector<std::set<enum domain_type>> domainGrid;
    std::vector<enum domain_type> assignmentGrid;
};

#endif /* FLOW_SOLVER_H */


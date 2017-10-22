/*
 *  csp_solver.h
 *
 *  Project: MP2
 *  Author : Christian J Howard
 *  Date   : 10/10/17
 *  Purpose:
 *
 */



#ifndef SRC_CSP_SOLVER_H
#define SRC_CSP_SOLVER_H

#include <map>

namespace csp {

    template<   typename X,
                typename D,
                template<typename> typename C,
                typename impl>
    class solver {
    public:

        // useful typedefs
        typedef X variables;
        typedef D domains;
        typedef C<X> constraints;
        typedef std::map<size_t, typename X::value> assignment;

        // solver state
        struct csp_state {
            variables   x;
            domains     d;
            constraints c;
        };

        // ctor/dtor
        solver()    = default;
        ~solver()   = default;

        // methods to setup and execute csp solver
        assignment solve( csp_state & csp_ , bool & success);

    private:

#define meta(func) static_cast<impl*>(this)->func

        bool isValueConsistent( size_t var,
                                const typename X::value & value,
                                const assignment & assignment_,
                                csp_state & csp_ ) const
        {
            return csp_.c.satisfyConstraints(var, value, assignment_);
        }

        // recursive back track function
        bool recursiveBackTrack( assignment & assignment_, csp_state & csp_ ){

            // temp variables
            bool success = false;
            assignment inference_list;

            // if the assignment has the same number of assignments as
            // the number of variables, we know we are done
            if( assignment_.size() == X::size() ){ return true; }

            // get the next state to assign
            size_t var = meta(selectUnassignedVariable)(csp_);

            // loop through values in current var's domain list
            for(const auto & value: meta(orderDomainValues)(var, assignment_, csp_)){

                // clear the inference list
                inference_list.clear();

                // check that the kth value is consistent
                if( isValueConsistent(var, value, assignment_, csp_) ){

                    // assign variable the current value
                    assignment_[var] = value;

                    // perform inference to see if we can stop early
                    bool isEarlyFail = meta(inference)(var, value, csp_, inference_list);

                    // if we are not failing early
                    if( !isEarlyFail ){

                        // add inferences to assignment
                        for(auto iter = inference_list.begin(); iter != inference_list.end(); ++iter ){
                            assignment_[iter->first] = iter->second;
                        }

                        // backtracking
                        success = recursiveBackTrack(assignment_,csp_);

                        // return success if it occurs
                        if( success ){ return success; }

                    }// end if
                }// end if

                // remove value added
                auto it = assignment_.find(var);
                if( it != assignment_.end() ){ assignment_.erase(it); }

                // remove list of inference values
                for(auto iter = inference_list.begin(); iter != inference_list.end(); ){
                    assignment_.erase(iter++);
                }

            }// end for
        }

#undef meta

    };


    template<typename X, typename D, template<typename> typename C, typename impl>
    typename solver<X,D,C,impl>::assignment solver<X,D,C,impl>::solve( csp_state & csp_ , bool & success ) {
        assignment a;
        success = recursiveBackTrack(a,csp_);
        return a;
    }

}

#endif //SRC_CSP_SOLVER_H

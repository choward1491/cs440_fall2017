
/* 
 * File:   flow_io.h
 * Author: lukep
 *
 * Created on October 24, 2017, 2:27 AM
 */

#ifndef FLOW_IO_H
#define FLOW_IO_H

#include "flow_csp.h"

namespace flow_io {

    void loadFlow(const std::string & flow_file, csp::flow_csp & out_flow );
    void saveFlow(const std::string & flow_file, const csp::flow_csp & in_flow );
};

#endif /* FLOW_IO_H */


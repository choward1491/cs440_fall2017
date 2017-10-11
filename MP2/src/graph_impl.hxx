/*
 *  graph_impl.hxx
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/14/17
 *  Purpose:
 *
 */

#ifndef graph_impl_h
#define graph_impl_h

#define TEMPLATE_HEADER template<typename data, typename node_type>
#define GRAPH graph<data,node_type>

/*
std::vector<std::list<node_type>>   node_connectivity;
std::vector<data>                   node_data;
 */


TEMPLATE_HEADER
GRAPH::graph(unsigned int num_nodes):node_connectivity(num_nodes),node_data(num_nodes) {
    
    
}

// set/get number of nodes
TEMPLATE_HEADER
unsigned int GRAPH::getNumNodes() const {
    return node_data.size();
}

TEMPLATE_HEADER
void GRAPH::setNumNodes( unsigned int num_nodes ) {
    if( node_data.size() != num_nodes ){
        node_data.resize(num_nodes);
        node_connectivity.resize(num_nodes);
    }
}

// add node with associated data to graph
TEMPLATE_HEADER
void GRAPH::addNode( node_type node_id, const data & ndata ) {
    if( node_id > node_data.size() ){
        setNumNodes( node_id + 1 );
    }
    node_data[node_id] = ndata;
}

// specify a directed edge from node1 to node2
TEMPLATE_HEADER
void GRAPH::pair(node_type node1, node_type node2 ) {
    node_connectivity[node1].push_front(node2);
}

TEMPLATE_HEADER
void GRAPH::pairUndirected(node_type node1, node_type node2) {
    node_connectivity[node1].push_front(node2);
    node_connectivity[node2].push_front(node1);
}

// get data for a given node
TEMPLATE_HEADER
const data & GRAPH::getDataFor( node_type node_id ) const {
    return node_data[node_id];
}

TEMPLATE_HEADER
data & GRAPH::getDataFor( node_type node_id ) {
    return node_data[node_id];
}

// get outward directed connectivity list for a given node
TEMPLATE_HEADER
const std::list<node_type> & GRAPH::getConnectivityListFor( node_type node_id) const {
    return node_connectivity[node_id];
}

TEMPLATE_HEADER
std::list<node_type> & GRAPH::getConnectivityListFor( node_type node_id) {
    return node_connectivity[node_id];
}

TEMPLATE_HEADER
void GRAPH::clear() {
    node_data.clear();
    node_connectivity.clear();
}
    


#undef TEMPLATE_HEADER
#undef GRAPH

#endif /* graph_impl_h */

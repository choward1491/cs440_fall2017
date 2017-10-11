/*
 *  graph.hpp
 *
 *  Project: MP1
 *  Author : Christian J Howard
 *  Date   : 9/14/17
 *  Purpose:
 *
 */

#ifndef graph_h
#define graph_h

#include <vector>
#include <list>


// basic graph data structure with not associated algorithms built in
template<typename data, typename node_type = unsigned int>
class graph {
public:
    
    typedef std::list<node_type> node_list;
    typedef node_type id_type;
    
    // ctors/dtors
    graph(unsigned int num_nodes = 1);
    ~graph() = default;
    
    // set/get number of nodes
    unsigned int getNumNodes() const;
    void setNumNodes( unsigned int num_nodes );
    
    // add node with associated data to graph
    void addNode( node_type node_id, const data & node_data );
    
    // specify a directed edge from node1 to node2
    void pair(node_type node1, node_type node2 );
    
    // specify a directed edge to and from node1 to node2, making it essentially an undirected edge
    void pairUndirected(node_type node1, node_type node2);
    
    // get data for a given node
    const data & getDataFor( node_type node_id ) const;
    data & getDataFor( node_type node_id );
    
    // get outward directed connectivity list for a given node
    const node_list & getConnectivityListFor( node_type node_id) const;
    node_list & getConnectivityListFor( node_type node_id);
    
    // clear data in graph
    void clear();
    
private:
    std::vector<node_list>  node_connectivity;
    std::vector<data>       node_data;
    
};


#include "graph_impl.hxx"


#endif /* graph_h */

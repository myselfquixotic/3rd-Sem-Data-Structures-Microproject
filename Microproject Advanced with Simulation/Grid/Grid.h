#ifndef GRID_H
#define GRID_H

#include <vector>
#include <stdexcept>
#include "../CoreData.h"

class PowerGrid {
private:
    std::vector<Node> nodes_;
    std::vector<std::vector<Edge>> adj_; // Adjacency list for sparse grids

public:
    // Adds a new node (e.g., hospital, substation) to the graph
    void addNode(const Node& n);
    
    // Adds an undirected transmission line between two nodes
    void addEdge(int u, int v, double effort, double km, double failProb);
    
    // Returns the total number of nodes in the grid
    int size() const;
    
    // Read-only accessor for a specific node
    const Node& getNode(int id) const;
    
    // Read-only accessor for the neighbors of a specific node
    const std::vector<Edge>& neighbors(int id) const;
    
    // Mutator for the DynamicUpdate loop to change edge weights during a storm
    bool setEdgeEffort(int u, int v, double effort);
};

#endif
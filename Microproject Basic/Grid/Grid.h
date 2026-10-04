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
    void addNode(const Node& n);
    void addEdge(int u, int v, double effort, double km, double failProb);
    
    int size() const;
    const Node& getNode(int id) const;
    const std::vector<Edge>& neighbors(int id) const;
    
    // Mutator for the DynamicUpdate loop to change edge weights during a storm
    bool setEdgeEffort(int u, int v, double effort);
};

#endif
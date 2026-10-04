#include "Grid.h"

void PowerGrid::addNode(const Node& n) {
    nodes_.push_back(n);
    // Expand the adjacency list so the indices perfectly match the node IDs
    adj_.push_back(std::vector<Edge>());
}

void PowerGrid::addEdge(int u, int v, double effort, double km, double failProb) {
    // Edge Case T3: Prevent negative weights for Dijkstra's algorithm
    if (effort < 0) {
        throw std::invalid_argument("Edge weight (effortHours) cannot be negative.");
    }
    
    // Since this is an undirected power grid, the line goes both ways.
    // We add the edge to both node u's list and node v's list.
    adj_[u].push_back({v, effort, km, failProb});
    adj_[v].push_back({u, effort, km, failProb});
}

int PowerGrid::size() const {
    return nodes_.size();
}

const Node& PowerGrid::getNode(int id) const {
    return nodes_[id];
}

const std::vector<Edge>& PowerGrid::neighbors(int id) const {
    return adj_[id];
}

bool PowerGrid::setEdgeEffort(int u, int v, double effort) {
    bool updated = false;
    
    // Update the u -> v direction
    for (Edge& edge : adj_[u]) {
        if (edge.to == v) {
            edge.effortHours = effort;
            updated = true;
            break;
        }
    }
    
    // Update the v -> u direction
    for (Edge& edge : adj_[v]) {
        if (edge.to == u) {
            edge.effortHours = effort;
            updated = true;
            break;
        }
    }
    
    return updated;
}
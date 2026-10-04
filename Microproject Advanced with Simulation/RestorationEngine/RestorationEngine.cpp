#include "RestorationEngine.h"
#include <queue>
#include <utility>

void RestorationEngine::init(int gridSize) {
    powered_.assign(gridSize, false);
}

void RestorationEngine::markPowered(int nodeId) {
    powered_[nodeId] = true;
}

bool RestorationEngine::isPowered(int nodeId) const {
    return powered_[nodeId];
}

std::vector<int> RestorationEngine::shortestPath(const PowerGrid& grid, const std::vector<int>& liveSources, int target) const {
    // Uses engine's own local arrays so the grid remains unchanged
    std::vector<double> dist(grid.size(), kInfinity);
    std::vector<int> prev(grid.size(), -1);
    
    // Min-heap for Dijkstra: pair<distance, nodeId>
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> minHeap;

    // Start Dijkstra from all currently powered live sources simultaneously
    for (int s : liveSources) {
        dist[s] = 0.0;
        minHeap.push({0.0, s});
    }

    // Invariant: every node already popped has its final (smallest) distance.
    while (!minHeap.empty()) {
        auto [d, u] = minHeap.top(); // Structured binding (C++17)
        minHeap.pop();

        if (d > dist[u]) continue; // Skip stale entries to prevent infinite cyclic redundancy

        if (u == target) break; // Target reached

        // Read-only access to neighbors
        for (const Edge& e : grid.neighbors(u)) {
            if (dist[u] + e.effortHours < dist[e.to]) {
                dist[e.to] = dist[u] + e.effortHours;
                prev[e.to] = u;
                minHeap.push({dist[e.to], e.to});
            }
        }
    }

    std::vector<int> path;
    
    // Edge Case 1: The Isolated Node (Target is unreachable)
    if (dist[target] == kInfinity) {
        return path; // Return empty vector gracefully
    }

    // Backtrack to rebuild the path
    for (int at = target; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    
    // Reverse the path so it goes from Source -> Target
    for(size_t i = 0; i < path.size() / 2; ++i) {
        std::swap(path[i], path[path.size() - 1 - i]);
    }

    return path;
}

std::vector<RepairStep> RestorationEngine::primPlan(const PowerGrid& grid, const std::vector<bool>& powered) const {
    std::vector<RepairStep> plan;
    std::vector<bool> inTree = powered; // Start the MST from all powered nodes
    
    // Min-heap of edges: pair<effort, pair<from, to>>
    std::priority_queue<std::pair<double, std::pair<int, int>>, 
                        std::vector<std::pair<double, std::pair<int, int>>>, 
                        std::greater<>> edgeHeap;

    // Push all edges extending outward from powered nodes
    for (int i = 0; i < grid.size(); ++i) {
        if (inTree[i]) {
            for (const Edge& e : grid.neighbors(i)) {
                if (!inTree[e.to]) {
                    edgeHeap.push({e.effortHours, {i, e.to}});
                }
            }
        }
    }

    while (!edgeHeap.empty()) {
        auto [effort, nodes] = edgeHeap.top();
        int u = nodes.first;
        int v = nodes.second;
        edgeHeap.pop();

        if (inTree[v]) continue; // Skip if both ends are powered (prevents cycles)

        plan.push_back({u, v, effort});
        inTree[v] = true; // Mark as part of the tree

        // Push new lines extending from the newly connected node
        for (const Edge& e : grid.neighbors(v)) {
            if (!inTree[e.to]) {
                edgeHeap.push({e.effortHours, {v, e.to}});
            }
        }
    }

    return plan;
}
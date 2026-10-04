#ifndef RESTORATION_ENGINE_H
#define RESTORATION_ENGINE_H

#include "../Grid/Grid.h"
#include "../Priority/Priority.h"
#include <vector>

constexpr double kInfinity = 1e9; // Standard coding convention constant

struct RepairStep {
    int u;
    int v;
    double effort;
};

class RestorationEngine {
private:
    std::vector<bool> powered_; // The engine's own progress record

public:
    // Initializes the powered status array based on grid size
    void init(int gridSize);

    // Marks a node as powered internally
    void markPowered(int nodeId);
    
    bool isPowered(int nodeId) const;

    // Phase 3: Emergency routing for critical sites (Priority 50+)
    std::vector<int> shortestPath(const PowerGrid& grid, const std::vector<int>& liveSources, int target) const;

    // Phase 4: General recovery for residential sites
    std::vector<RepairStep> primPlan(const PowerGrid& grid, const std::vector<bool>& powered) const;
};

#endif
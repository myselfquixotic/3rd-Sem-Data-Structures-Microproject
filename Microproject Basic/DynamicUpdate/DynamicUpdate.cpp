#include "DynamicUpdate.h"

DynamicUpdate::DynamicUpdate(unsigned int seed) : rng_(seed) {}

bool DynamicUpdate::triggerCyclone(PowerGrid& grid, int category, double& currentTime) {
    std::cout << "\n[!!!] WARNING: REPEAT CYCLONE DETECTED (Category " << category << ")\n";
    std::cout << "[-] Crews pulling back to shelter. Adding 4.0 hours to system clock.\n";

    // Enforce constraint: Crews must shelter for 4 hours during the storm
    currentTime += 4.0; 
    
    bool damageOccurred = false;
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int u = 0; u < grid.size(); ++u) {
        const std::vector<Edge>& neighbors = grid.neighbors(u);

        for (const Edge& edge : neighbors) {
            int v = edge.to;

            // Only roll the dice once per physical undirected line
            if (u < v) {
                double r = dist(rng_);
                double effectiveFailProb = edge.failProb * category;

                if (r < effectiveFailProb) {
                    // Weather event hit this line! Increase the repair effort.
                    double newEffort = edge.effortHours + (2.5 * category); 
                    
                    grid.setEdgeEffort(u, v, newEffort);
                    
                    std::cout << "[-] Line (" << u << " -> " << v 
                              << ") damaged by storm! Repair effort increased to " 
                              << newEffort << " hours.\n";

                    damageOccurred = true;
                }
            }
        }
    }

    if (damageOccurred) {
        std::cout << "[-] Active repairs cancelled.\n";
        std::cout << "[*] Rebuilding Priority Queue... \n";
    } else {
        std::cout << "[*] Grid withstood the storm. No new damage occurred.\n";
    }

    return damageOccurred;
}
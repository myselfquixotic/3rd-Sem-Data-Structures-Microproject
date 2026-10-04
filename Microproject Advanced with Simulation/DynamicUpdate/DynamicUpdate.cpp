#include "DynamicUpdate.h"

DynamicUpdate::DynamicUpdate(unsigned int seed) : rng_(seed) {}

bool DynamicUpdate::triggerCyclone(PowerGrid& grid, int category, double& currentTime) {
    std::cout << "\n[!!!] WARNING: REPEAT CYCLONE DETECTED (Category " << category << ")\n";
    std::cout << "[-] Crews pulling back to shelter. Adding 4.0 hours to system clock.\n";

    // Enforce the hard constraint: Crews must shelter for 4 hours during the storm
    currentTime += 4.0; 
    
    bool damageOccurred = false;
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Iterate through all nodes in the grid
    for (int u = 0; u < grid.size(); ++u) {
        
        // Fetch read-only neighbors to maintain low coupling
        const std::vector<Edge>& neighbors = grid.neighbors(u);

        for (const Edge& edge : neighbors) {
            int v = edge.to;

            // Since the graph is undirected, every line exists twice (u->v and v->u).
            // We only roll the dice once per physical line by checking if u < v.
            if (u < v) {
                double r = dist(rng_);
                
                // The cyclone category acts as a multiplier to the base failure probability
                double effectiveFailProb = edge.failProb * category;

                if (r < effectiveFailProb) {
                    // Weather event hit this line! Increase the repair effort.
                    // A real Category 3/4 storm drastically increases the needed crew-hours.
                    double newEffort = edge.effortHours + (2.5 * category); 
                    
                    // Update the graph safely through the public interface mutator
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
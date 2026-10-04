#include <tuple>
#include <sstream>
#include <queue>
#include <limits>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <thread>
#include <chrono>
#include "CoreData.h"
#include "Grid/Grid.h"
#include "Priority/Priority.h"
#include "RestorationEngine/RestorationEngine.h"
#include "DynamicUpdate/DynamicUpdate.h"

// ---------------------------------------------------------
// GRAPH CONSTRUCTION HELPER FUNCTIONS
// ---------------------------------------------------------
void addRealisticEdge(PowerGrid& grid, int u, int v, double failProb) {
    const Node& nodeU = grid.getNode(u);
    const Node& nodeV = grid.getNode(v);

    double pixelDist = std::sqrt(std::pow(nodeU.x - nodeV.x, 2) + std::pow(nodeU.y - nodeV.y, 2));
    double distanceKm = pixelDist / 60.0;
    double effortHours = 0.5 + (distanceKm * 0.3);

    grid.addEdge(u, v, effortHours, distanceKm, failProb);
}

void buildCityGraph(PowerGrid& grid) {
    grid.addNode({0, "Tabahi Power Plant", 100, 0.0, true, 222, 91});
    grid.addNode({1, "Transmission Substation T1", 100, 0.0, false, 580, 154});
    grid.addNode({2, "Transmission Substation T2", 100, 0.0, false, 1366, 125});
    grid.addNode({3, "Transmission Substation T3", 100, 0.0, false, 1481, 745});
    grid.addNode({4, "Transmission Substation T4", 100, 0.0, false, 539, 639});
    grid.addNode({5, "District Substation D1", 100, 0.0, false, 292, 280});
    grid.addNode({6, "District Substation D2", 100, 0.0, false, 1038, 252});
    grid.addNode({7, "District Substation D3", 100, 0.0, false, 1467, 335});
    grid.addNode({8, "District Substation D4", 100, 0.0, false, 1652, 210});
    grid.addNode({9, "District Substation D5", 100, 0.0, false, 1670, 528});
    grid.addNode({10, "District Substation D6", 100, 0.0, false, 1249, 708});
    grid.addNode({11, "District Substation D7", 100, 0.0, false, 800, 493});
    grid.addNode({12, "District Substation D8", 100, 0.0, false, 450, 724});
    
    grid.addNode({13, "Lohith Hospital", 90, 4.0, false, 348, 505});
    grid.addNode({14, "Vikas Hospital", 90, 6.0, false, 1214, 259});
    grid.addNode({15, "Ramesh Hospital", 90, 9.0, false, 1020, 847});
    grid.addNode({16, "Municipality Water Plant", 90, 5.0, false, 1025, 70});
    grid.addNode({17, "Water Pump 1", 90, 8.0, false, 1045, 754});
    grid.addNode({18, "Water Pump 2", 90, 8.0, false, 1338, 564});
    grid.addNode({19, "Jio Telecom", 90, 12.0, false, 792, 100});
    grid.addNode({20, "Airtel Telecom", 90, 12.0, false, 145, 778});
    grid.addNode({21, "Vadapav Telecom", 90, 12.0, false, 1340, 909});
    grid.addNode({22, "Aag Laga Denge Fire Station", 90, 24.0, false, 994, 618});
    grid.addNode({23, "Dabangg Police Station", 90, 24.0, false, 1135, 471});
    grid.addNode({24, "Mohan Airport", 90, 48.0, false, 556, 904});
    
    grid.addNode({25, "Cyclone Shelter 1", 70, 0.0, false, 146, 576});
    grid.addNode({26, "Cyclone Shelter 2", 70, 0.0, false, 791, 787});
    grid.addNode({27, "Ambani Port", 70, 0.0, false, 1771, 435});
    grid.addNode({28, "Tejaswi Railway Station", 70, 0.0, false, 974, 410});
    
    grid.addNode({29, "IndianOil Fuel Depot", 50, 0.0, false, 1787, 744});
    grid.addNode({30, "Vishal Mall", 30, 0.0, false, 901, 337});
    grid.addNode({31, "Money Follows My Brother University", 30, 0.0, false, 584, 357});
    
    grid.addNode({32, "Dholakpur (Residential)", 10, 0.0, false, 665, 242});
    grid.addNode({33, "Furfuri Nagar (Residential)", 10, 0.0, false, 1293, 425});
    grid.addNode({34, "Eden Gardens (Residential)", 10, 0.0, false, 1514, 669});
    grid.addNode({35, "Chandapur (Residential)", 10, 0.0, false, 753, 646});
    grid.addNode({36, "Forbesganj Industrial Area", 10, 0.0, false, 1717, 91});
    grid.addNode({37, "Lolpur Industrial Area", 10, 0.0, false, 1704, 886});

    addRealisticEdge(grid, 0, 1, 0.1); addRealisticEdge(grid, 0, 2, 0.1); addRealisticEdge(grid, 1, 3, 0.2);
    addRealisticEdge(grid, 2, 4, 0.2); addRealisticEdge(grid, 3, 4, 0.1); addRealisticEdge(grid, 1, 5, 0.3);
    addRealisticEdge(grid, 1, 6, 0.3); addRealisticEdge(grid, 2, 7, 0.3); addRealisticEdge(grid, 3, 8, 0.3);
    addRealisticEdge(grid, 3, 9, 0.3); addRealisticEdge(grid, 4, 10, 0.3); addRealisticEdge(grid, 4, 11, 0.3);
    addRealisticEdge(grid, 2, 12, 0.3); addRealisticEdge(grid, 5, 6, 0.2); addRealisticEdge(grid, 7, 8, 0.2);
    addRealisticEdge(grid, 10, 11, 0.2); addRealisticEdge(grid, 5, 13, 0.1); addRealisticEdge(grid, 6, 13, 0.1);
    addRealisticEdge(grid, 7, 14, 0.1); addRealisticEdge(grid, 12, 14, 0.1); addRealisticEdge(grid, 8, 15, 0.1);
    addRealisticEdge(grid, 9, 15, 0.1); addRealisticEdge(grid, 10, 16, 0.1); addRealisticEdge(grid, 11, 16, 0.1);
    addRealisticEdge(grid, 6, 17, 0.2); addRealisticEdge(grid, 11, 18, 0.2); addRealisticEdge(grid, 5, 19, 0.2);
    addRealisticEdge(grid, 8, 20, 0.2); addRealisticEdge(grid, 12, 21, 0.2); addRealisticEdge(grid, 7, 22, 0.1);
    addRealisticEdge(grid, 9, 23, 0.1); addRealisticEdge(grid, 4, 24, 0.1); addRealisticEdge(grid, 6, 25, 0.2);
    addRealisticEdge(grid, 9, 26, 0.2); addRealisticEdge(grid, 12, 27, 0.3); addRealisticEdge(grid, 8, 28, 0.3);
    addRealisticEdge(grid, 10, 29, 0.3); addRealisticEdge(grid, 7, 30, 0.4); addRealisticEdge(grid, 11, 31, 0.3);
    addRealisticEdge(grid, 13, 32, 0.5); addRealisticEdge(grid, 25, 33, 0.5); addRealisticEdge(grid, 14, 34, 0.5);
    addRealisticEdge(grid, 26, 35, 0.5); addRealisticEdge(grid, 16, 36, 0.5); addRealisticEdge(grid, 31, 37, 0.5);
    addRealisticEdge(grid, 32, 33, 0.4); addRealisticEdge(grid, 34, 35, 0.4); addRealisticEdge(grid, 36, 37, 0.4);
}

// ---------------------------------------------------------
// ALGORITHM & QUEUE HELPERS
// ---------------------------------------------------------
void populateQueue(PriorityQueue& pq, const PowerGrid& grid, const RestorationEngine& engine) {
    std::vector<RepairTask> tasks;
    for (int i = 0; i < grid.size(); ++i) {
        if (!engine.isPowered(i) && !grid.getNode(i).isPowered) {
            const Node& n = grid.getNode(i);
            tasks.push_back({n.id, n.priorityLevel, n.fuelHoursLeft, 0.0});
        }
    }
    pq.rebuild(tasks);
}

double getEdgeEffort(const PowerGrid& grid, int u, int v) {
    for (const auto& edge : grid.neighbors(u)) {
        if (edge.to == v) return edge.effortHours;
    }
    return 0.0;
}

std::vector<int> findPath(const PowerGrid& grid, int startNode, int targetNode) {
    std::vector<double> dist(grid.size(), std::numeric_limits<double>::infinity());
    std::vector<int> parent(grid.size(), -1);
    using pii = std::pair<double, int>;
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;

    dist[startNode] = 0.0;
    pq.push({0.0, startNode});

    while (!pq.empty()) {
        double d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;
        if (u == targetNode) break;

        for (const auto& edge : grid.neighbors(u)) {
            if (dist[u] + edge.effortHours < dist[edge.to]) {
                dist[edge.to] = dist[u] + edge.effortHours;
                parent[edge.to] = u;
                pq.push({dist[edge.to], edge.to});
            }
        }
    }

    std::vector<int> path;
    for (int at = targetNode; at != -1; at = parent[at]) {
        path.push_back(at);
    }
    std::reverse(path.begin(), path.end());
    
    if (path.size() == 1 && path[0] != startNode) return {}; 
    return path;
}

// ---------------------------------------------------------
// MAIN EXECUTION PIPELINE
// ---------------------------------------------------------
int main() {
    std::cout << "===================================================\n";
    std::cout << " CYCLONE GRID RESTORATION - DECISION SUPPORT SYSTEM\n";
    std::cout << "===================================================\n\n";

    PowerGrid grid;
    buildCityGraph(grid);
    std::cout << "[SYSTEM] City Map Loaded: " << grid.size() << " Nodes, 48 Lines. Population: 880,000.\n";

    RestorationEngine engine;
    engine.init(grid.size());
    engine.markPowered(0); // Power Plant is the live source

    PriorityQueue pq;
    populateQueue(pq, grid, engine);

    // Seed the RNG for Dynamic Update
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::srand(seed); 
    DynamicUpdate stormSimulator(seed); 
    
    double currentTime = 0.0;
    int cyclonesHit = 0;
    int targetRepeatCyclones = std::rand() % 4; 
    int severeStormCount = 0; 
    
    std::cout << "[!] GRID ONLINE. ALL SYSTEMS NOMINAL.\n";
    std::cout << "    Monitoring for meteorological disturbances...\n\n";

    currentTime = 24.0;
    std::cout << "[!] WARNING: CATEGORY 4 CYCLONE LANDFALL DETECTED at T+24.0 hrs.\n";
    std::cout << "    Grid disconnected. Commencing repairs...\n\n";

    // Intercept and filter the inaccurate "rebuilding" output for the INITIAL storm only
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    
    stormSimulator.triggerCyclone(grid, 4, currentTime); 
    severeStormCount++; 
    double lastCycloneTime = currentTime; 
    
    std::cout.rdbuf(oldCout); 
    std::string outLine;
    while (std::getline(buffer, outLine)) {
        if (outLine.find("REPEAT CYCLONE DETECTED") != std::string::npos) continue;
        if (outLine.find("Crews pulling back") != std::string::npos) continue;
        if (outLine.find("Active repairs cancelled") != std::string::npos) continue;
        if (outLine.find("Rebuilding Priority Queue") != std::string::npos) continue;
        std::cout << outLine << "\n";
    } 
    
    populateQueue(pq, grid, engine);

    // Initialize 10 independent repair crews starting at the Power Plant
    std::vector<Crew> crews;
    for (int i = 1; i <= 10; ++i) {
        crews.push_back({i, currentTime, 0});
    }

    // Process nodes based on Priority Queue
    while (!pq.empty()) {
        RepairTask currentTask = pq.tryPop().value();
        
        // --- PHASE 4: PRIM'S ALGORITHM HYBRID SHIFT ---
        if (currentTask.priorityLevel < 50) {
            std::cout << "\n===================================================\n";
            std::cout << " [!] CRITICAL SITES SECURED (PRIORITY < 50).\n";
            std::cout << " [!] SWITCHING TO PRIM'S ALGORITHM FOR MASS CONNECTION.\n";
            std::cout << "===================================================\n\n";

            std::vector<int> unpoweredNodes;
            unpoweredNodes.push_back(currentTask.nodeId);
            while (!pq.empty()) {
                unpoweredNodes.push_back(pq.tryPop().value().nodeId);
            }

            std::vector<bool> inMST(grid.size(), true);
            for (int id : unpoweredNodes) {
                inMST[id] = false;
            }

            using PrimEdge = std::tuple<double, int, int>; 
            std::priority_queue<PrimEdge, std::vector<PrimEdge>, std::greater<PrimEdge>> primPQ;

            for (int i = 0; i < grid.size(); ++i) {
                if (inMST[i]) {
                    for (const auto& e : grid.neighbors(i)) {
                        if (!inMST[e.to]) {
                            primPQ.push({e.effortHours, i, e.to});
                        }
                    }
                }
            }

            // Build the Minimum Spanning Tree and dispatch crews
            while (!primPQ.empty()) {
                auto [effort, u, v] = primPQ.top();
                primPQ.pop();

                if (inMST[v]) continue; 
                inMST[v] = true; 

                for (const auto& e : grid.neighbors(v)) {
                    if (!inMST[e.to]) {
                        primPQ.push({e.effortHours, v, e.to});
                    }
                }

                int bestCrewIdx = 0;
                for (int i = 1; i < 10; ++i) {
                    if (crews[i].availableTime < crews[bestCrewIdx].availableTime) {
                        bestCrewIdx = i;
                    }
                }
                Crew& activeCrew = crews[bestCrewIdx];
                if (activeCrew.availableTime < lastCycloneTime) activeCrew.availableTime = lastCycloneTime;

                double taskStart = activeCrew.availableTime;
                double taskEnd = taskStart + effort;

                std::cout << "--- DISPATCHING CREW " << activeCrew.id << " (PRIM'S MST) ---\n";
                std::cout << "[Routing] Building Grid Backbone: Node " << u << " -> Node " << v << " (" << std::fixed << std::setprecision(1) << effort << " hrs)\n";
                std::cout << "[+] POWER RESTORED to " << grid.getNode(v).name << " at T+" << taskEnd << " hours.\n\n";

                activeCrew.availableTime = taskEnd;
                activeCrew.currentNode = v;
                if (taskEnd > currentTime) currentTime = taskEnd;
            }
            break; 
        }

        // --- PHASE 3: DIJKSTRA'S ALGORITHM ROUTING ---
        int bestCrewIdx = 0;
        for (int i = 1; i < 10; ++i) { 
            if (crews[i].availableTime < crews[bestCrewIdx].availableTime) {
                bestCrewIdx = i;
            }
        }
        Crew& activeCrew = crews[bestCrewIdx];

        if (activeCrew.availableTime < lastCycloneTime) {
            activeCrew.availableTime = lastCycloneTime;
            activeCrew.currentNode = 0; 
        }

        std::vector<int> path = findPath(grid, activeCrew.currentNode, currentTask.nodeId);
        
        // Failsafe: If isolated, fallback to starting from the Power Plant
        if (path.empty()) {
            path = findPath(grid, 0, currentTask.nodeId);
            activeCrew.currentNode = 0;
        }

        double pathEffort = 0;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            pathEffort += getEdgeEffort(grid, path[i], path[i+1]);
        }

        double taskStart = activeCrew.availableTime;
        double taskEnd = taskStart + pathEffort;

        std::cout << "--- DISPATCHING CREW " << activeCrew.id << " (DIJKSTRA) ---\n";
        std::cout << "[Queue] Extracting target: " << grid.getNode(currentTask.nodeId).name << "\n";
        std::cout << "[Routing] Crew " << activeCrew.id << " departing from Node " << activeCrew.currentNode << ".\n";
        std::cout << "[Routing] Path found. Estimated Effort: " << std::fixed << std::setprecision(1) << pathEffort << " hours.\n";
        
        activeCrew.availableTime = taskEnd;
        activeCrew.currentNode = currentTask.nodeId;
        if (taskEnd > currentTime) currentTime = taskEnd;
        
        std::cout << "[+] POWER RESTORED to " << grid.getNode(currentTask.nodeId).name 
                  << " at T+" << taskEnd << " hours.\n\n";

        // --- PHASE 5: DYNAMIC DISRUPTION ---
        if (cyclonesHit < targetRepeatCyclones && (currentTime - lastCycloneTime > 48.0) && (std::rand() % 100) < 20) { 
            
            // Enforce (Cat 3 + Cat 4 <= 2) across the entire simulation
            int category;
            if (severeStormCount >= 2) {
                category = (std::rand() % 2) + 1; 
            } else {
                category = (std::rand() % 4) + 1;
                if (category >= 3) {
                    severeStormCount++;
                }
            }
            
            bool gridChanged = stormSimulator.triggerCyclone(grid, category, currentTime);
            
            if (gridChanged) {
                cyclonesHit++;
                lastCycloneTime = currentTime; 
                populateQueue(pq, grid, engine); 
            }
        }
    }

    std::cout << "===================================================\n";
    std::cout << " RESTORATION COMPLETE.\n";
    std::cout << " Total Elapsed Time: " << std::fixed << std::setprecision(1) << currentTime << " hours.\n";
    std::cout << " Total Cyclones Endured: " << cyclonesHit + 1 << "\n";
    std::cout << "===================================================\n";

    return 0;
}
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
#include <fstream>
#include "CoreData.h"
#include "Grid/Grid.h"
#include "Priority/Priority.h"
#include "RestorationEngine/RestorationEngine.h"
#include "DynamicUpdate/DynamicUpdate.h"

// Calculates real-world distance and repair effort based on pixel coordinates
void addRealisticEdge(PowerGrid& grid, int u, int v, double failProb) {
    const Node& nodeU = grid.getNode(u);
    const Node& nodeV = grid.getNode(v);

    // 1. Calculate the straight-line distance in pixels
    double pixelDist = std::sqrt(std::pow(nodeU.x - nodeV.x, 2) + std::pow(nodeU.y - nodeV.y, 2));

    // 2. Convert pixels to kilometers (Scale: ~60 pixels = 1 km)
    double distanceKm = pixelDist / 60.0;

    // 3. Calculate effort hours (Base 0.5 hours + 0.3 hours per km of line)
    double effortHours = 0.5 + (distanceKm * 0.3);

    // 4. Add the edge to the graph
    grid.addEdge(u, v, effortHours, distanceKm, failProb);
}

// ---------------------------------------------------------
// Helper: Builds the 38-Node, 47-Line City
// ---------------------------------------------------------
void buildCityGraph(PowerGrid& grid) {
    // Format: addNode({id, "Name", priority, fuel, isPowered, x, y})

    // Priority 100: Substations & Power Plant
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
    // ---------------------------------------------------------
    // ---------------------------------------------------------
    // 48 UPDATED CONNECTIONS (Dynamic Distance & Effort)
    // Format: addRealisticEdge(grid, from, to, failProb)
    // ---------------------------------------------------------
    // Core Transmission Ring
    addRealisticEdge(grid, 0, 1, 0.1);
    addRealisticEdge(grid, 0, 2, 0.1);
    addRealisticEdge(grid, 1, 3, 0.2);
    addRealisticEdge(grid, 2, 4, 0.2);
    addRealisticEdge(grid, 3, 4, 0.1);

    // Distribution Feeders
    addRealisticEdge(grid, 1, 5, 0.3);
    addRealisticEdge(grid, 1, 6, 0.3);
    addRealisticEdge(grid, 2, 7, 0.3);
    addRealisticEdge(grid, 3, 8, 0.3);
    addRealisticEdge(grid, 3, 9, 0.3);
    addRealisticEdge(grid, 4, 10, 0.3);
    addRealisticEdge(grid, 4, 11, 0.3);
    addRealisticEdge(grid, 2, 12, 0.3);

    // District Links
    addRealisticEdge(grid, 5, 6, 0.2);
    addRealisticEdge(grid, 7, 8, 0.2);
    addRealisticEdge(grid, 10, 11, 0.2);

    // Critical Infrastructure Feeds
    addRealisticEdge(grid, 5, 13, 0.1);
    addRealisticEdge(grid, 6, 13, 0.1);
    addRealisticEdge(grid, 7, 14, 0.1);
    addRealisticEdge(grid, 12, 14, 0.1);
    addRealisticEdge(grid, 8, 15, 0.1);
    addRealisticEdge(grid, 9, 15, 0.1);
    addRealisticEdge(grid, 10, 16, 0.1);
    addRealisticEdge(grid, 11, 16, 0.1);
    addRealisticEdge(grid, 6, 17, 0.2);
    addRealisticEdge(grid, 11, 18, 0.2);
    addRealisticEdge(grid, 5, 19, 0.2);
    addRealisticEdge(grid, 8, 20, 0.2);
    addRealisticEdge(grid, 12, 21, 0.2);
    addRealisticEdge(grid, 7, 22, 0.1);
    addRealisticEdge(grid, 9, 23, 0.1);
    addRealisticEdge(grid, 4, 24, 0.1);

    // Shelters, Logistics, and Community
    addRealisticEdge(grid, 6, 25, 0.2);
    addRealisticEdge(grid, 9, 26, 0.2);
    addRealisticEdge(grid, 12, 27, 0.3);
    addRealisticEdge(grid, 8, 28, 0.3);
    addRealisticEdge(grid, 10, 29, 0.3);
    addRealisticEdge(grid, 7, 30, 0.4);
    addRealisticEdge(grid, 11, 31, 0.3);

    // Residential & Industrial Feeds
    addRealisticEdge(grid, 13, 32, 0.5);
    addRealisticEdge(grid, 25, 33, 0.5);
    addRealisticEdge(grid, 14, 34, 0.5);
    addRealisticEdge(grid, 26, 35, 0.5);
    addRealisticEdge(grid, 16, 36, 0.5);
    addRealisticEdge(grid, 31, 37, 0.5);

    // Final Residential Tie-Lines
    addRealisticEdge(grid, 32, 33, 0.4);
    addRealisticEdge(grid, 34, 35, 0.4);
    addRealisticEdge(grid, 36, 37, 0.4);
}

// ---------------------------------------------------------
// Helper: Populates the Priority Queue with Unpowered Nodes
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

// HELPER 1: Finds the effort cost between two specific nodes
double getEdgeEffort(const PowerGrid& grid, int u, int v) {
    for (const auto& edge : grid.neighbors(u)) {
        if (edge.to == v) return edge.effortHours;
    }
    return 0.0;
}

// HELPER 2: Reconstructs the exact path (e.g., [0, 1, 5, 13]) to draw the yellow lines
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
    
    // If no path found (completely isolated), return empty
    if (path.size() == 1 && path[0] != startNode) return {}; 
    return path;
}

// ---------------------------------------------------------
// Main Execution Pipeline
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

    // Seed the standard random generator with high-resolution time so rapid executions don't copy seeds
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::srand(seed); 

    // Seed Mohan's dynamic simulator with the same high-res seed
    DynamicUpdate stormSimulator(seed); 
    
    double currentTime = 0.0;
    int cyclonesHit = 0;
    
    // RESTORED: Randomly choose 0 to 3 repeat cyclones (Total = 1 initial + 0 to 3 repeats)
    int targetRepeatCyclones = std::rand() % 4; 
    
    // NEW: Tracker to ensure (Cat 3 + Cat 4) <= 2
    int severeStormCount = 0; 
    
    std::vector<std::string> eventLog;
    
    // 1. Log the peaceful start at Hour 0.0
    eventLog.push_back("{\"type\": \"start\", \"time\": 0.0}");

    std::cout << "[!] GRID ONLINE. ALL SYSTEMS NOMINAL.\n";
    std::cout << "    Monitoring for meteorological disturbances...\n\n";

    // 2. Advance time to 24.0 hours (Peaceful period)
    currentTime = 24.0;
    
    std::cout << "[!] WARNING: CATEGORY 4 CYCLONE LANDFALL DETECTED at T+24.0 hrs.\n";
    std::cout << "    Grid disconnected. Commencing repairs...\n\n";

    // NEW: Save the exact start time so the visualizer triggers instantly at 24.0
    double cycloneStartTime = currentTime;

    // 3. Trigger the initial Category 4 storm to physically break lines
    std::vector<double> preStormEffort;
    for(int u=0; u<grid.size(); ++u) {
        for(auto& e : grid.neighbors(u)) {
            if(u < e.to) preStormEffort.push_back(e.effortHours);
        }
    }

    // NEW: Intercept and filter the inaccurate output for the FIRST storm only
    std::stringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    
    stormSimulator.triggerCyclone(grid, 4, currentTime); 
    
    std::cout.rdbuf(oldCout); // Restore standard output
    
    std::string outLine;
    while (std::getline(buffer, outLine)) {
        // Filter out the incorrect repeat/cancellation messages for the initial storm
        if (outLine.find("REPEAT CYCLONE DETECTED") != std::string::npos) continue;
        if (outLine.find("Crews pulling back") != std::string::npos) continue;
        if (outLine.find("Active repairs cancelled") != std::string::npos) continue;
        if (outLine.find("Rebuilding Priority Queue") != std::string::npos) continue;
        std::cout << outLine << "\n";
    } 
    
    // NEW: We just triggered a Category 4, so we increment the severe storm counter
    severeStormCount++; 
    
    // NEW: Cooldown tracker so repeat cyclones don't overlap visually
    double lastCycloneTime = currentTime; 
    
    std::string brokenStr = "[";
    int edgeIdx = 0;
    bool firstBrk = true;
    for(int u=0; u<grid.size(); ++u) {
        for(auto& e : grid.neighbors(u)) {
            if(u < e.to) {
                if(e.effortHours > preStormEffort[edgeIdx] + 1.0) { 
                    if(!firstBrk) brokenStr += ",";
                    brokenStr += "[" + std::to_string(u) + "," + std::to_string(e.to) + "]";
                    firstBrk = false;
                }
                edgeIdx++;
            }
        }
    }
    brokenStr += "]";

    // Log the initial cyclone using the exact start time (24.0)
    eventLog.push_back("{\"type\": \"cyclone\", \"time\": " + std::to_string(cycloneStartTime) + ", \"broken\": " + brokenStr + "}");
    
    // Refresh the queue since edge weights just changed from the storm
    populateQueue(pq, grid, engine);

    // NEW: Initialize 10 independent repair crews starting at the Power Plant
    std::vector<Crew> crews;
    for (int i = 1; i <= 10; ++i) {
        crews.push_back({i, cycloneStartTime, 0});
    }

    while (!pq.empty()) {
        RepairTask currentTask = pq.tryPop().value();
        
        // --- NEW: THE PRIM'S ALGORITHM HYBRID SHIFT ---
        if (currentTask.priorityLevel < 50) {
            std::cout << "\n===================================================\n";
            std::cout << " [!] CRITICAL SITES SECURED (PRIORITY < 50).\n";
            std::cout << " [!] SWITCHING TO PRIM'S ALGORITHM FOR MASS CONNECTION.\n";
            std::cout << "===================================================\n\n";

            // 1. Find all remaining unpowered nodes by emptying the priority queue
            std::vector<int> unpoweredNodes;
            unpoweredNodes.push_back(currentTask.nodeId);
            while (!pq.empty()) {
                unpoweredNodes.push_back(pq.tryPop().value().nodeId);
            }

            // 2. Everything NOT in the unpowered list forms our powered baseline
            std::vector<bool> inMST(grid.size(), true);
            for (int id : unpoweredNodes) {
                inMST[id] = false;
            }

            // 3. Initialize Prim's Priority Queue with the powered boundary edges
            using PrimEdge = std::tuple<double, int, int>; // <effort, from, to>
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

            // 4. Build the Minimum Spanning Tree and dispatch crews
            while (!primPQ.empty()) {
                auto [effort, u, v] = primPQ.top();
                primPQ.pop();

                if (inMST[v]) continue; // Skip if already connected
                inMST[v] = true; // Mark as successfully connected to the backbone

                // Add new boundary edges to the queue
                for (const auto& e : grid.neighbors(v)) {
                    if (!inMST[e.to]) {
                        primPQ.push({e.effortHours, v, e.to});
                    }
                }

                // Dispatch the earliest available crew to build this specific MST edge
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

                std::string pathStr = "[" + std::to_string(u) + "," + std::to_string(v) + "]";

                eventLog.push_back("{\"type\": \"repair_start\", \"time\": " + std::to_string(taskStart) + 
                                   ", \"target\": " + std::to_string(v) + 
                                   ", \"crew\": " + std::to_string(activeCrew.id) + 
                                   ", \"path\": " + pathStr + "}");

                // --- PRIM'S CONSOLE OUTPUT ---
                std::cout << "--- DISPATCHING CREW " << activeCrew.id << " (PRIM'S MST) ---\n";
                std::cout << "[Routing] Building Grid Backbone: Node " << u << " -> Node " << v << " (" << std::fixed << std::setprecision(1) << effort << " hrs)\n";
                std::cout << "[+] POWER RESTORED to " << grid.getNode(v).name << " at T+" << taskEnd << " hours.\n\n";

                activeCrew.availableTime = taskEnd;
                activeCrew.currentNode = v;
                if (taskEnd > currentTime) currentTime = taskEnd;

                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                eventLog.push_back("{\"type\": \"repair_end\", \"time\": " + std::to_string(taskEnd) + 
                                   ", \"target\": " + std::to_string(v) + 
                                   ", \"crew\": " + std::to_string(activeCrew.id) + 
                                   ", \"path\": " + pathStr + "}");
            }
            
            // The grid is fully reconnected. Break the main simulation loop.
            break; 
        }

        // Find the earliest available crew out of all 10
        int bestCrewIdx = 0;
        for (int i = 1; i < 10; ++i) { // Loop checks indices 0 through 9
            if (crews[i].availableTime < crews[bestCrewIdx].availableTime) {
                bestCrewIdx = i;
            }
        }
        Crew& activeCrew = crews[bestCrewIdx];

        // Force crew time to sync with storms if they were sheltered
        if (activeCrew.availableTime < lastCycloneTime) {
            activeCrew.availableTime = lastCycloneTime;
            activeCrew.currentNode = 0; // Return to base after sheltering
        }

        // Route the crew from WHERE THEY ARE to the target node
        std::vector<int> path = findPath(grid, activeCrew.currentNode, currentTask.nodeId);
        
        // Failsafe: If isolated, fallback to starting from the Power Plant
        if (path.empty()) {
            path = findPath(grid, 0, currentTask.nodeId);
            activeCrew.currentNode = 0;
        }
        
        std::string pathStr = "[";
        for (size_t i = 0; i < path.size(); ++i) {
            pathStr += std::to_string(path[i]);
            if (i < path.size() - 1) pathStr += ",";
        }
        pathStr += "]";

        double pathEffort = 0;
        for (size_t i = 0; i < path.size() - 1; ++i) {
            pathEffort += getEdgeEffort(grid, path[i], path[i+1]);
        }

        // Calculate parallel timeline
        double taskStart = activeCrew.availableTime;
        double taskEnd = taskStart + pathEffort;

        eventLog.push_back("{\"type\": \"repair_start\", \"time\": " + std::to_string(taskStart) + ", \"target\": " + std::to_string(currentTask.nodeId) + ", \"crew\": " + std::to_string(activeCrew.id) + ", \"path\": " + pathStr + "}");

        // --- MULTI-CREW CONSOLE OUTPUT ---
        std::cout << "--- DISPATCHING CREW " << activeCrew.id << " (DIJKSTRA) ---\n";
        std::cout << "[Queue] Extracting target: " << grid.getNode(currentTask.nodeId).name << "\n";
        std::cout << "[Routing] Crew " << activeCrew.id << " departing from Node " << activeCrew.currentNode << ".\n";
        std::cout << "[Routing] Path found. Estimated Effort: " << pathEffort << " hours.\n";
        
        // Advance global clock and crew status
        activeCrew.availableTime = taskEnd;
        activeCrew.currentNode = currentTask.nodeId;
        if (taskEnd > currentTime) currentTime = taskEnd;
        
        std::cout << "[+] POWER RESTORED to " << grid.getNode(currentTask.nodeId).name 
                  << " at T+" << std::fixed << std::setprecision(1) << taskEnd << " hours.\n\n";
                  
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); 
        // --------------------------------

        eventLog.push_back("{\"type\": \"repair_end\", \"time\": " + std::to_string(taskEnd) + ", \"target\": " + std::to_string(currentTask.nodeId) + ", \"crew\": " + std::to_string(activeCrew.id) + ", \"path\": " + pathStr + "}");

        // Phase 5: Dynamic Disruption
        if (cyclonesHit < targetRepeatCyclones && (currentTime - lastCycloneTime > 48.0) && (std::rand() % 100) < 20) { 
            
            // NEW: Enforce (Cat 3 + Cat 4 <= 2) across the entire simulation
            int category;
            if (severeStormCount >= 2) {
                // If we already had 2 severe storms, force this one to be a Cat 1 or Cat 2
                category = (std::rand() % 2) + 1; 
            } else {
                // Otherwise, it can be anything from Cat 1 to Cat 4
                category = (std::rand() % 4) + 1;
                
                // If the randomizer picked a 3 or 4, count it
                if (category >= 3) {
                    severeStormCount++;
                }
            }
            
            // Capture effort BEFORE storm to detect what broke
            std::vector<double> preStormEffort;
            for(int u=0; u<grid.size(); ++u) {
                for(auto& e : grid.neighbors(u)) {
                    if(u < e.to) preStormEffort.push_back(e.effortHours);
                }
            }

            double repeatStartTime = currentTime;
            bool gridChanged = stormSimulator.triggerCyclone(grid, category, currentTime);
            
            if (gridChanged) {
                cyclonesHit++;
                lastCycloneTime = currentTime; // Reset the cooldown
                
                std::string brokenStr = "[";
                int edgeIdx = 0;
                bool firstBrk = true;
                for(int u=0; u<grid.size(); ++u) {
                    for(auto& e : grid.neighbors(u)) {
                        if(u < e.to) {
                            if(e.effortHours > preStormEffort[edgeIdx] + 1.0) { // Penalty detected
                                if(!firstBrk) brokenStr += ",";
                                brokenStr += "[" + std::to_string(u) + "," + std::to_string(e.to) + "]";
                                firstBrk = false;
                            }
                            edgeIdx++;
                        }
                    }
                }
                brokenStr += "]";

                eventLog.push_back("{\"type\": \"cyclone\", \"time\": " + std::to_string(repeatStartTime) + ", \"broken\": " + brokenStr + "}");
                
                populateQueue(pq, grid, engine); 
            }
        }
    }

    std::cout << "===================================================\n";
    std::cout << " RESTORATION COMPLETE.\n";
    std::cout << " Total Elapsed Time: " << std::fixed << std::setprecision(1) << currentTime << " hours.\n";
    std::cout << " Total Cyclones Endured: " << cyclonesHit + 1 << "\n";
    std::cout << "===================================================\n";
    // ---------------------------------------------------------
    // JSON EXPORT FOR WEB FRONTEND
    // ---------------------------------------------------------
    std::ofstream outFile("disaster_log.json");
    if (outFile.is_open()) {
        outFile << "{\n";
        
        // 1. Export Nodes
        outFile << "  \"nodes\": [\n";
        for (int i = 0; i < grid.size(); ++i) {
            const Node& n = grid.getNode(i);
            outFile << "    {\"id\": " << n.id 
                    << ", \"name\": \"" << n.name << "\""
                    << ", \"x\": " << n.x 
                    << ", \"y\": " << n.y 
                    << ", \"priority\": " << n.priorityLevel << "}";
            if (i < grid.size() - 1) outFile << ",";
            outFile << "\n";
        }
        outFile << "  ],\n";

        // 2. Export Edges (Power Lines)
        outFile << "  \"lines\": [\n";
        bool firstEdge = true;
        for (int u = 0; u < grid.size(); ++u) {
            for (const Edge& e : grid.neighbors(u)) {
                if (u < e.to) {
                    if (!firstEdge) outFile << ",\n";
                    outFile << "    {\"from\": " << u << ", \"to\": " << e.to << "}";
                    firstEdge = false;
                }
            }
        }
        outFile << "  ],\n"; // Added comma here

        // 3. Export Events (The Animation Timeline)
        outFile << "  \"events\": [\n";
        for (size_t i = 0; i < eventLog.size(); ++i) {
            outFile << "    " << eventLog[i];
            if (i < eventLog.size() - 1) outFile << ",";
            outFile << "\n";
        }
        outFile << "  ]\n";
        
        outFile << "}\n";
        outFile.close();
        std::cout << "[+] Exported timeline to disaster_log.json for the visualizer.\n";
    }

    return 0;
}
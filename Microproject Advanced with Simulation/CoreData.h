#ifndef CORE_DATA_H
#define CORE_DATA_H

#include <string>

// priority scale: 10 to 100
struct Node {
    int id;
    std::string name;
    int priorityLevel;    // e.g., 100 for Substation, 90 for Hospital
    double fuelHoursLeft; // Backup fuel limit (tie-breaker for critical nodes)
    bool isPowered;       // True if successfully connected to a power source
    int x, y;             // For visualization purposes (not used in algorithms)
};

struct Edge {
    int to;
    double effortHours;   // Edge weight: Repair effort
    double distanceKm;    // Physical length of the line
    double failProb;      // Chance of failing again in a secondary cyclone
};

struct Crew {
    int id;
    double availableTime;
    int currentNode;
};

#endif
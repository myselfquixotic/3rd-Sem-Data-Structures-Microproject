#ifndef CORE_DATA_H
#define CORE_DATA_H

#include <string>

// Priority scale: 10 to 100
struct Node {
    int id;
    std::string name;
    int priorityLevel;    
    double fuelHoursLeft; 
    bool isPowered;       
    int x, y;             // Map coordinates
};

struct Edge {
    int to;
    double effortHours;   // Edge weight: Repair effort
    double distanceKm;    // Physical length of the line
    double failProb;      // Vulnerability to secondary cyclones
};

struct Crew {
    int id;
    double availableTime;
    int currentNode;
};

#endif
#ifndef DYNAMIC_UPDATE_H
#define DYNAMIC_UPDATE_H

#include "../CoreData.h"
#include "../Grid/Grid.h"
#include <random>
#include <iostream>

class DynamicUpdate {
private:
    std::mt19937 rng_; // Seeded random number generator for repeatable tests

public:
    // Constructor initializes the RNG with a fixed seed (e.g., 42)
    DynamicUpdate(unsigned int seed = 42);

    // Triggers a cyclone, damages lines based on probability, and advances time
    // Returns true if any lines were damaged so the main loop knows to rebuild the queue
    bool triggerCyclone(PowerGrid& grid, int category, double& currentTime);
};

#endif
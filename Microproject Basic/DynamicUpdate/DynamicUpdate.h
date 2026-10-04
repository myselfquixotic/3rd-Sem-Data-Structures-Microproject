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
    DynamicUpdate(unsigned int seed = 42);

    // Triggers a cyclone, damages lines based on probability, and advances time
    bool triggerCyclone(PowerGrid& grid, int category, double& currentTime);
};

#endif
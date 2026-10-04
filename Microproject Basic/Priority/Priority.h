#ifndef PRIORITY_H
#define PRIORITY_H

#include <vector>
#include <optional>
#include <stdexcept>

// A Task is independent of the grid structure to maintain high cohesion
struct RepairTask {
    int nodeId;
    int priorityLevel;
    double fuelHoursLeft;
    double repairTime;

    // Max-heap operator overload
    bool operator<(const RepairTask& other) const {
        if (priorityLevel != other.priorityLevel) {
            return priorityLevel < other.priorityLevel; 
        }
        // Tie-breaker 1: Smaller fuel hours left is MORE urgent (so we use > to reverse it for max-heap)
        if (fuelHoursLeft != other.fuelHoursLeft) {
            return fuelHoursLeft > other.fuelHoursLeft; 
        }
        // Tie-breaker 2: Shorter repair time is MORE urgent
        return repairTime > other.repairTime;
    }
};

class PriorityQueue {
private:
    std::vector<RepairTask> heap_;

    // Hidden helpers for array-based heap management
    void siftUp(int i);
    void siftDown(int i);

public:
    void push(const RepairTask& t);
    const RepairTask& top() const;
    RepairTask pop();
    
    // Safely handles Edge Case 2 (Queue Underflow)
    std::optional<RepairTask> tryPop(); 
    
    bool empty() const;
    
    // O(n) heapify for Phase 5 dynamic disruption loop
    void rebuild(const std::vector<RepairTask>& tasks); 
};

#endif
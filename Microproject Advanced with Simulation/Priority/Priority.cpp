#include "Priority.h"
#include <algorithm>

void PriorityQueue::siftUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap_[parent] < heap_[i]) {
            std::swap(heap_[parent], heap_[i]);
            i = parent;
        } else {
            break;
        }
    }
}

void PriorityQueue::siftDown(int i) {
    int n = heap_.size();
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;

        if (left < n && heap_[largest] < heap_[left]) largest = left;
        if (right < n && heap_[largest] < heap_[right]) largest = right;

        if (largest != i) {
            std::swap(heap_[i], heap_[largest]);
            i = largest;
        } else {
            break;
        }
    }
}

void PriorityQueue::push(const RepairTask& t) {
    heap_.push_back(t);
    siftUp(heap_.size() - 1);
}

const RepairTask& PriorityQueue::top() const {
    if (heap_.empty()) throw std::out_of_range("Queue is empty");
    return heap_.front();
}

RepairTask PriorityQueue::pop() {
    if (heap_.empty()) throw std::out_of_range("Queue is empty");
    RepairTask root = heap_.front();
    heap_.front() = heap_.back();
    heap_.pop_back();
    if (!heap_.empty()) siftDown(0);
    return root;
}

std::optional<RepairTask> PriorityQueue::tryPop() {
    if (heap_.empty()) {
        return std::nullopt; // Fulfills Edge Case 2 graceful handling
    }
    return pop();
}

bool PriorityQueue::empty() const {
    return heap_.empty();
}

void PriorityQueue::rebuild(const std::vector<RepairTask>& tasks) {
    heap_ = tasks;
    // Bottom-up heapify runs in O(n) time
    for (int i = (heap_.size() / 2) - 1; i >= 0; --i) {
        siftDown(i);
    }
}
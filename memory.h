#pragma once
#include <vector>
#include <list>

class MemoryManager {
public:
    enum class Policy { FIFO, LRU };
    MemoryManager(int frames, int pageSize);
    void simulateReferences(const std::vector<int>& refs, Policy p);
private:
    int framesCount;
    int pageSize;
};

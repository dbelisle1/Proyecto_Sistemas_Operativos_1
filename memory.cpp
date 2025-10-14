#include "memory.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <queue>

MemoryManager::MemoryManager(int frames, int pageSize): framesCount(frames), pageSize(pageSize) {}

void MemoryManager::simulateReferences(const std::vector<int>& refs, Policy p) {
    std::cout << "Simulating references with policy " << (p==Policy::FIFO?"FIFO":"LRU") << "\n";
    int faults = 0;
    if (p==Policy::FIFO) {
        std::queue<int> q;
        std::unordered_set<int> inmem;
        for (int r: refs) {
            if (inmem.find(r)==inmem.end()) {
                faults++;
                if ((int)q.size() == framesCount) {
                    int out = q.front(); q.pop(); inmem.erase(out);
                    std::cout<<"Evicting (FIFO): "<<out<<"\n";
                }
                q.push(r); inmem.insert(r);
                std::cout<<"Load page "<<r<<" -> fault\n";
            } else {
                std::cout<<"Hit page "<<r<<"\n";
            }
        }
    } else { // LRU
        std::list<int> lru; // front = most recent
        std::unordered_map<int, std::list<int>::iterator> pos;
        for (int r: refs) {
            auto it = pos.find(r);
            if (it==pos.end()) {
                faults++;
                if ((int)lru.size() == framesCount) {
                    int victim = lru.back(); lru.pop_back(); pos.erase(victim);
                    std::cout<<"Evicting (LRU): "<<victim<<"\n";
                }
                lru.push_front(r); pos[r]=lru.begin();
                std::cout<<"Load page "<<r<<" -> fault\n";
            } else {
                // move to front
                lru.erase(it->second);
                lru.push_front(r);
                pos[r]=lru.begin();
                std::cout<<"Hit page "<<r<<"\n";
            }
        }
    }
    std::cout<<"Total page faults: "<<faults<<"\n";
}

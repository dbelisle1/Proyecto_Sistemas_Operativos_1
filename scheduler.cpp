#include "scheduler.h"
#include <iostream>

Scheduler::Scheduler() {}

void Scheduler::addProcess(const Process &p) {
    q.push(p);
}

void Scheduler::runFCFS() {
    int now = 0;
    std::cout << "Running FCFS...\n";
    while(!q.empty()) {
        Process p = q.front(); q.pop();
        if (now < p.arrival) now = p.arrival;
        std::cout << "PID " << p.pid << " starts at " << now << " runs for " << p.burst << "\n";
        now += p.burst;
        std::cout << "PID " << p.pid << " finishes at " << now << "\n";
    }
}

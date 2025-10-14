#pragma once
#include <queue>
#include <vector>

struct Process {
    int pid;
    int arrival; // tiempo de llegada
    int burst; // ráfaga
};

class Scheduler {
public:
    Scheduler();
    void addProcess(const Process &p);
    void runFCFS();
private:
    std::queue<Process> q;
};

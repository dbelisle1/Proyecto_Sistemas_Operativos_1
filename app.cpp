#include "app.h"
#include <iostream>

App::App() : scheduler(), pc(), mem(4, 8), tlb(4) {}
App::~App() {}

void App::runConsoleDemo() {
    std::cout << "=== SimuladorSO - demo console ===\n";
    // Añadimos procesos de ejemplo
    scheduler.addProcess({1, 0, 5});
    scheduler.addProcess({2, 1, 3});
    scheduler.addProcess({3, 2, 7});

    scheduler.runFCFS();

    // Demo productor-consumidor (corre por un corto tiempo)
    pc.start();
    sleep(2);
    pc.stop();

    // Demo memoria: referencia de páginas
    std::vector<int> refs = {1,2,3,2,4,1,5,2,3,4};
    mem.simulateReferences(refs, MemoryManager::Policy::FIFO);
    mem.simulateReferences(refs, MemoryManager::Policy::LRU);
}

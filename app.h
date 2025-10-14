#pragma once

#include "scheduler.h"
#include "producer_consumer.h"
#include "memory.h"
#include "tlb.h"

class App {
public:
    App();
    ~App();

    void runConsoleDemo();

    Scheduler scheduler;
    ProducerConsumer pc;
    MemoryManager mem;
    TLB tlb;
};

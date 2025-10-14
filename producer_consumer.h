#pragma once
#include <vector>
#include <atomic>
#include <semaphore.h>  // para sem_t, sem_init, sem_wait, sem_post
#include <pthread.h>    // para pthread_t
#include <unistd.h>     // para sleep()

class ProducerConsumer {
public:
    ProducerConsumer();
    ~ProducerConsumer();
    void start();
    void stop();
private:
    static void* producerThread(void* arg);
    static void* consumerThread(void* arg);
    void runProducer();
    void runConsumer();

    std::atomic<bool> running{false};
    pthread_t prod, cons;
    static const int BUFFER_SIZE = 8;
    int buffer[BUFFER_SIZE];
    int frontIdx{0}, backIdx{0};
    sem_t empty, full, mutex;
};

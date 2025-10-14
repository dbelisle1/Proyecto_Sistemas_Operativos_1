#include "producer_consumer.h"
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <iostream>

ProducerConsumer::ProducerConsumer() {
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);
}
ProducerConsumer::~ProducerConsumer() {
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
}

void ProducerConsumer::start() {
    running = true;
    pthread_create(&prod, nullptr, &ProducerConsumer::producerThread, this);
    pthread_create(&cons, nullptr, &ProducerConsumer::consumerThread, this);
}

void ProducerConsumer::stop() {
    running = false;
    pthread_join(prod, nullptr);
    pthread_join(cons, nullptr);
}

void* ProducerConsumer::producerThread(void* arg) {
    ProducerConsumer* self = (ProducerConsumer*)arg;
    self->runProducer();
    return nullptr;
}

void* ProducerConsumer::consumerThread(void* arg) {
    ProducerConsumer* self = (ProducerConsumer*)arg;
    self->runConsumer();
    return nullptr;
}

void ProducerConsumer::runProducer() {
    int counter = 1;
    while(running) {
        sem_wait(&empty);
        sem_wait(&mutex);
        buffer[backIdx] = counter++;
        std::cout << "Produced: " << buffer[backIdx] << " at idx " << backIdx << "\n";
        backIdx = (backIdx + 1) % BUFFER_SIZE;
        sem_post(&mutex);
        sem_post(&full);
        sleep(1);
    }
}

void ProducerConsumer::runConsumer() {
    while(running) {
        sem_wait(&full);
        sem_wait(&mutex);
        int val = buffer[frontIdx];
        std::cout << "Consumed: " << val << " from idx " << frontIdx << "\n";
        frontIdx = (frontIdx + 1) % BUFFER_SIZE;
        sem_post(&mutex);
        sem_post(&empty);
        sleep(2);
    }
}

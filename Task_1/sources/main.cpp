#include <iostream>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <random>
#include <string>

#define NUM_READERS 10
#define NUM_WRITERS 20

#define READER_TIME 1
#define WRITER_TIME 2

#define REPEATS_COUNT 3

pthread_mutex_t printMutex;

void print(const std::string &msg) {
    pthread_mutex_lock(&printMutex);
    std::cout << msg << std::endl;
    pthread_mutex_unlock(&printMutex);
}

size_t readers = 0;
size_t writers = 0;

pthread_mutex_t mutexR;
pthread_mutex_t mutexW;

pthread_cond_t readingEnd;
pthread_cond_t writingEnd;

pthread_barrier_t barrier;

void *read(void *str) {
    auto pthreadId = pthread_self();
    print(std::string(" Reader ") + std::to_string(pthreadId) +
          std::string(" start"));

    pthread_mutex_lock(&mutexR);
    while (writers != 0) {
        pthread_cond_wait(&writingEnd, &mutexR);
    }
    ++readers;
    pthread_mutex_unlock(&mutexR);

    print(std::string(" Reader ") + std::to_string(pthreadId) +
          std::string(" got access"));
    sleep(READER_TIME);
    print(std::string("read: ") + *static_cast<std::string *>(str));
    print(std::string(" Reader ") + std::to_string(pthreadId) +
          std::string(" finish work"));

    pthread_mutex_lock(&mutexR);
    --readers;
    if (readers == 0) {
        pthread_cond_broadcast(&readingEnd);
    }
    pthread_mutex_unlock(&mutexR);

    pthread_barrier_wait(&barrier);

    return nullptr;
}

void *write(void *args) {
    auto pthreadId = pthread_self();
    print(std::string(" Writer ") + std::to_string(pthreadId) +
          std::string(" start"));

    pthread_mutex_lock(&mutexW);
    ++writers;
    while (readers != 0) {
        pthread_cond_wait(&readingEnd, &mutexW);
    }

    print(std::string(" Writer ") + std::to_string(pthreadId) +
          std::string(" GOT access"));
    sleep(WRITER_TIME);
    auto str = static_cast<std::string *>(args);
    *str += "@";
    print(std::string("write: ") + *str);
    print(std::string(" Writer ") + std::to_string(pthreadId) +
          std::string(" FINISH work"));

    --writers;
    if (writers == 0) {
        pthread_cond_broadcast(&writingEnd);
    }
    pthread_mutex_unlock(&mutexW);

    pthread_barrier_wait(&barrier);

    return nullptr;
}

int main() {
    std::vector<pthread_t> threads(NUM_READERS + NUM_WRITERS);

    pthread_mutex_init(&mutexR, nullptr);
    pthread_mutex_init(&mutexW, nullptr);

    pthread_cond_init(&writingEnd, nullptr);
    pthread_cond_init(&readingEnd, nullptr);

    pthread_barrier_init(&barrier, nullptr, NUM_READERS + NUM_WRITERS + 1);

    std::random_device rd;
    std::mt19937 mersenne(rd());
    auto *str = new std::string;
    for (size_t repeat = 1; repeat <= REPEATS_COUNT; ++repeat) {
        std::cout << "Iteration №" << repeat << std::endl;
        size_t numWriters = 0, numReaders = 0;
        *str = "@";
        while (numWriters < NUM_WRITERS || numReaders < NUM_READERS) {
            if (mersenne() % 2 == 0 && numWriters < NUM_WRITERS) {
                if (pthread_create(&threads[numWriters + numReaders], nullptr, write, static_cast<void *>(str))) {
                    std::cout << "Error! Thread cant be created!";
                    return -1;
                }
                ++numWriters;
            } else if (numReaders < NUM_READERS) {
                if (pthread_create(&threads[numWriters + numReaders], nullptr, read, static_cast<void *>(str))) {
                    std::cout << "Error! Thread cant be created!";
                    return -1;
                }
                ++numReaders;
            }
        }

        pthread_barrier_wait(&barrier);
        for (size_t thread : threads) {
            pthread_join(thread, nullptr);
        }

        std::cout << std::endl;
    }
    delete str;
    pthread_barrier_destroy(&barrier);
    pthread_cond_destroy(&writingEnd);
    pthread_cond_destroy(&readingEnd);
    pthread_mutex_destroy(&mutexR);
    pthread_mutex_destroy(&mutexW);

    return 0;
}
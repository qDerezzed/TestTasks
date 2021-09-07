#include <iostream>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <random>
#include <string>
#include <queue>

#define NUM_READERS 5
#define NUM_WRITERS 10

#define READER_TIME 1
#define WRITER_TIME 2

#define REPEATS_COUNT 3

class Pool {
public:
    Pool() = delete;

    ~Pool() {
        cancelled = true;
        pthread_mutex_lock(&mutex);
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
        for (size_t i = 0; i < amountThreads; i++) {
            pthread_join(threads[i], nullptr);
        }
    }

    Pool(void *(*inputFunction)(void *), size_t inputAmountThreads) :
            pFunction(inputFunction), amountThreads(inputAmountThreads) {
        threads.resize(amountThreads);
        pthread_mutex_init(&mutex, nullptr);
        pthread_cond_init(&cond, nullptr);
        for (size_t i = 0; i < amountThreads; i++) {
            pthread_create(&threads[i], nullptr, &Pool::thread, this);
        }
    }

    void addToPool(void *arg) {
        pthread_mutex_lock(&mutex);
        queue.push(arg);
        remaining++;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    void wait() {
        pthread_mutex_lock(&mutex);
        while (!cancelled && remaining) {
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);
    }

private:
    bool cancelled = false;

    void *(*pFunction)(void *);

    size_t remaining = 0;
    size_t amountThreads;
    std::queue<void *> queue;
    pthread_mutex_t mutex{};
    pthread_cond_t cond{};
    std::vector<pthread_t> threads;

    static void *thread(void *inputPool) {
        auto pool = static_cast<Pool *>(inputPool);
        while (!pool->cancelled) {
            pthread_mutex_lock(&pool->mutex);
            while (!pool->cancelled && pool->queue.empty()) {
                pthread_cond_wait(&pool->cond, &pool->mutex);
            }
            if (pool->cancelled) {
                pthread_mutex_unlock(&pool->mutex);
                return nullptr;
            }

            auto temp = pool->queue.front();
            pool->queue.pop();

            pthread_mutex_unlock(&pool->mutex);

            pool->pFunction(temp);

            pthread_mutex_lock(&pool->mutex);
            pool->remaining--;
            pthread_cond_broadcast(&pool->cond);
            pthread_mutex_unlock(&pool->mutex);
        }

        return nullptr;
    }
};

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


    return nullptr;
}

int main() {
    pthread_mutex_init(&mutexR, nullptr);
    pthread_mutex_init(&mutexW, nullptr);

    pthread_cond_init(&writingEnd, nullptr);
    pthread_cond_init(&readingEnd, nullptr);

    std::random_device rd;
    std::mt19937 mersenne(rd());
    auto *str = new std::string;
    Pool writersPool(write, NUM_WRITERS);
    Pool readersPool(read, NUM_READERS);
    for (size_t repeat = 1; repeat <= REPEATS_COUNT; ++repeat) {
        std::cout << "Iteration №" << repeat << std::endl;
        size_t numWriters = 0, numReaders = 0;
        *str = "@";
        while (numWriters < NUM_WRITERS || numReaders < NUM_READERS) {
            if (mersenne() % 2 == 0 && numWriters < NUM_WRITERS) {
                writersPool.addToPool(static_cast<void *>(str));
                ++numWriters;
            } else if (numReaders < NUM_READERS) {
                readersPool.addToPool(static_cast<void *>(str));
                ++numReaders;
            }
        }
        writersPool.wait();
        readersPool.wait();
        std::cout << std::endl;
    }
    delete str;
    pthread_cond_destroy(&writingEnd);
    pthread_cond_destroy(&readingEnd);
    pthread_mutex_destroy(&mutexR);
    pthread_mutex_destroy(&mutexW);
    return 0;
}

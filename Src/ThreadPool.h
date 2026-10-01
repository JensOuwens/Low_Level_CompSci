
#ifndef MYGAME_THREADPOOL_H
#define MYGAME_THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {

public:
    ThreadPool(size_t num_threads = std::thread::hardware_concurrency());

    ~ThreadPool();

    void enqueue(std::function<void()> task);

    void waitUntilFinished();

private:
    std::vector<std::thread> threads;

    std::queue<std::function<void()>> tasks_;

    std::mutex queue_mutex_;

    std::condition_variable cv_;
    std::condition_variable idle_cv_;

    size_t active_tasks_ = 0;

    bool stop_ = false;
};

#endif
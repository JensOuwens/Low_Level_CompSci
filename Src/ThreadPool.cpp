
#include "ThreadPool.h"

#include <stdexcept>

ThreadPool::ThreadPool(size_t num_threads)
{
    // Ensure at least one worker exists.
    if (num_threads == 0)
        num_threads = 1;

    for (size_t i = 0; i < num_threads; ++i)
    {
        threads.emplace_back([this]
        {
            while (true)
            {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(queue_mutex_);

                    cv_.wait(lock, [this]
                    {
                        return !tasks_.empty() || stop_;
                    });

                    if (stop_ && tasks_.empty())
                        return;

                    task = std::move(tasks_.front());
                    tasks_.pop();

                    ++active_tasks_;
                }

                // Execute task outside the queue lock.
                try
                {
                    task();
                }
                catch (...)
                {
                    // Prevent an exception from terminating the worker.
                    // Production code should report task failures.
                }

                {
                    std::lock_guard<std::mutex> lock(queue_mutex_);

                    --active_tasks_;

                    if (tasks_.empty() && active_tasks_ == 0)
                    {
                        idle_cv_.notify_all();
                    }
                }
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        stop_ = true;
    }

    cv_.notify_all();

    for (auto& thread : threads)
    {
        if (thread.joinable())
            thread.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task)
{
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);

        if (stop_)
            throw std::runtime_error("Cannot enqueue task: pool is stopping");

        tasks_.emplace(std::move(task));
    }

    cv_.notify_one();
}

void ThreadPool::waitUntilFinished()
{
    std::unique_lock<std::mutex> lock(queue_mutex_);

    idle_cv_.wait(lock, [this]
    {
        return tasks_.empty() && active_tasks_ == 0;
    });
}
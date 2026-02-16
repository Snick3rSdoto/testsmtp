#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t threadCount)
    : running(true)
{
    for (size_t i = 0; i < threadCount; ++i)
    {
        workers.emplace_back(&ThreadPool::worker, this);
    }
}

ThreadPool::~ThreadPool()
{
    stop();
}

void ThreadPool::enqueue(std::function<void()> task)
{
    {
        std::lock_guard<std::mutex> lock(mutex);
        tasks.push(std::move(task));
    }
    condition.notify_one();
}

void ThreadPool::stop()
{
    running = false;
    condition.notify_all();

    for (auto& t : workers)
    {
        if (t.joinable())
            t.join();
    }
}

void ThreadPool::worker()
{
    while (running)
    {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(mutex);
            condition.wait(lock, [this] {
                return !tasks.empty() || !running;
            });

            if (!running && tasks.empty())
                return;

            task = std::move(tasks.front());
            tasks.pop();
        }

        task();
    }
}


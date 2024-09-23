#pragma once

#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <future>
#include <condition_variable>

class ThreadPool 
{
public:

    ThreadPool(size_t threads);
    void wait();
    ~ThreadPool();

    // Templated enqueue function to accept any callable
    template <class F>
    void enqueue(F&& f);

private:

    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::vector<std::future<void>> futures;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop = false;

};

// Add tasks to the queue
template<class F>
void ThreadPool::enqueue(F&& f)
{
    using return_type = typename std::result_of<F()>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(std::forward<F>(f));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // Don't allow enqueueing after stopping the pool
        if (stop) 
        {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }

        tasks.emplace([task]() { (*task)(); });
        futures.push_back(std::move(res));  // Store the future for later use in wait()
    }
    condition.notify_one(); 
}




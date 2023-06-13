#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

std::queue<int> eventQueue;

std::mutex mutex;
std::condition_variable cv;

void processEvent(int event)
{
    std::cout << "Processing event: " << event << std::endl;
}

void eventLoop()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mutex);
        std::cerr << "Waiting... \n";

        cv.wait(lock, [] { return !eventQueue.empty(); });

        int event = eventQueue.front();
        eventQueue.pop();

        lock.unlock();

        processEvent(event);
    }
}

int main()
{
    std::thread loopThread(eventLoop);

    for (int i = 0; i < 10; ++i)
    {
        std::lock_guard<std::mutex> lock(mutex);
        eventQueue.push(i);

        cv.notify_one();
    }

    loopThread.join();

    return 0;
}

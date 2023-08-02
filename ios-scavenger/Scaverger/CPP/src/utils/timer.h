//
// Created by liruopeng on 2023/7/28.
//

#ifndef ANDROID_SCAVENGER_TIMER_H
#define ANDROID_SCAVENGER_TIMER_H

#include <iostream>
#include <queue>
#include <functional>
#include <chrono>
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>


class Timer {
private:
    typedef std::chrono::duration<int, std::ratio<1, 1000000>> DurationMicrosT;
    mutable std::mutex m_MuxLock;
    using MuxGuard = std::lock_guard<std::mutex>;
    using MuxUniqLck = std::unique_lock<std::mutex>;
    std::condition_variable m_timerCond;
    std::condition_variable m_workerCond;
    std::thread m_timerThread;
    std::thread m_workerThread;
    std::atomic<bool> m_timerMainThreadAlive;
    std::atomic<bool> m_timerStarted;

    std::atomic <uint32_t> m_callbackSn;
    std::function<void()> m_callbackFunc = std::function<void()>(nullptr);
    std::chrono::microseconds m_sleepMicros;
    std::queue <uint32_t> m_workQueue;
    std::queue <uint32_t> m_timerQueue;
    std::chrono::steady_clock::time_point m_startTimePoint;
    std::chrono::steady_clock::time_point m_nowTimePoint;
    DurationMicrosT m_deltaMicros;
private:
    void create();

    void destroy();

public:
    Timer() : m_timerMainThreadAlive(false) {
        m_timerStarted = false;
        m_callbackSn = (uint32_t) - 1;
        m_sleepMicros = std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::milliseconds(1000));
        m_callbackFunc = std::function<void()>(nullptr);
        create();
    }

    ~Timer() {
        destroy();
    }

    Timer(const Timer &) = delete;

    Timer &operator=(const Timer &) = delete;

public:

    void start(int intervalmillis, std::function<void()> callback);

    bool timerWait();

    void doNotify();

    void workWait();

    void doCallback();

    void stop();
};


#endif //ANDROID_SCAVENGER_TIMER_H

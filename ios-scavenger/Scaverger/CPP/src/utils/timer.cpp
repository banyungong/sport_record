//
// Created by liruopeng on 2023/7/28.
//

#include "timer.h"

void Timer::create() {
    if (m_timerMainThreadAlive) {
        return;
    }
    m_timerMainThreadAlive = true;
    m_timerThread = std::thread([this]() { //lambda
        do {
            if (timerWait()) {
                doNotify(); // notify work thread
            }
        } while (m_timerMainThreadAlive);
    });

    m_workerThread = std::thread([this]() {
        do {
            workWait(); // wait for timer notify
            doCallback();
        } while (m_timerMainThreadAlive);
    });
}

void Timer::destroy() {
    m_timerMainThreadAlive = false;
    stop();
    m_workerCond.notify_all();
    m_timerCond.notify_all();
    if (m_timerThread.joinable()) {
        m_timerThread.join();
    }
    if (m_workerThread.joinable()) {
        m_workerThread.join();
    }
}

void Timer::start(int intervalmillis, std::function<void()> callback) {
    std::chrono::milliseconds interval_millis(intervalmillis);
    std::chrono::microseconds interval_micros = std::chrono::duration_cast<std::chrono::microseconds>(
            interval_millis);
    std::chrono::microseconds adjust_micros(500);
    std::chrono::microseconds sleep_micros(interval_micros - adjust_micros);
    {
        MuxGuard g(m_MuxLock);
        while (!m_workQueue.empty()) {
            m_workQueue.pop();
        }
        m_timerStarted = true;
        m_callbackSn = 0;
        m_callbackFunc = callback;
        m_sleepMicros = sleep_micros;
        m_timerQueue.push(m_callbackSn);
        if (!m_timerMainThreadAlive) {
            create();
        }
    }
    m_timerCond.notify_one();
}

bool Timer::timerWait() {
    MuxUniqLck tul(m_MuxLock);
    uint32_t sn;
    if (!m_timerQueue.empty()) {
        while (!m_timerQueue.empty()) {
            m_timerQueue.pop();
        }
        return false;
    }

    sn = m_callbackSn;
    m_startTimePoint = std::chrono::steady_clock::now();
    m_timerCond.wait_for(tul, std::chrono::microseconds(m_sleepMicros));
    if (!m_timerStarted) {
        return false;
    }
    if (sn != m_callbackSn) {
        return false;
    }
    m_nowTimePoint = std::chrono::steady_clock::now();
    m_deltaMicros = std::chrono::duration_cast<DurationMicrosT>(m_nowTimePoint - m_startTimePoint);
    if (m_deltaMicros.count() < m_sleepMicros.count() / 2) {
        return false;
    }
    m_timerQueue.push(sn);
    return true;
}

void Timer::doNotify() {
    {
        MuxGuard g(m_MuxLock);
        uint32_t sn = 0;
        if (m_timerQueue.empty()) {
            return;
        }
        sn = m_timerQueue.front();
        m_timerQueue.pop();
        if (sn != m_callbackSn) {
            return;
        }
        if (!m_timerStarted) {
            return;
        }
        m_callbackSn += 1;
        m_workQueue.push(m_callbackSn);
    }
    m_workerCond.notify_one();
}

void Timer::workWait() {
    MuxUniqLck wul(m_MuxLock);
    while (m_workQueue.empty() && m_timerMainThreadAlive) {
        m_workerCond.wait(wul);
    }
}

void Timer::doCallback() {
    std::function<void()> callback = std::function<void()>(nullptr);
    {
        MuxGuard g(m_MuxLock);
        uint32_t sn = 0;
        while (!m_workQueue.empty()) {
            sn = m_workQueue.front();
            m_workQueue.pop();
        };
        if (sn == 0) {
            return;
        }
        if (!m_timerStarted) {
            return;
        }
        if (sn != m_callbackSn) {
            return;
        }
        if (!m_callbackFunc) {
            return;
        }
        callback = m_callbackFunc;
    }
    try {
        callback();
    } catch (const std::bad_function_call &e) {
        (void) e;
    }
}

void Timer::stop() {
    {
        MuxGuard g(m_MuxLock);
        while (!m_workQueue.empty()) {
            m_workQueue.pop();
        }
        m_timerStarted = false;
        m_callbackSn = (uint32_t) -1;
        m_callbackFunc = std::function<void()>(nullptr);
    }
}

package com.gritti.scavenger.controls

import java.util.Timer
import java.util.TimerTask

class TimerControls {

    private var second = 0
    private var mTimer: Timer? = null

    private var mListeners = mutableListOf<OnTimerListener>()

    fun startTimer() {
        stopTimer()
        mTimer = Timer()
        mTimer?.schedule(object : TimerTask() {
            override fun run() {
                second++
                mListeners.forEach {
                    it.onTimer(second)
                }
            }
        }, 0, 999)
    }

    fun addListener(listener: OnTimerListener) {
        mListeners.add(listener)
    }

    fun removeListener(listener: OnTimerListener) {
        mListeners.remove(listener)
    }

    fun stopTimer() {
        mTimer?.cancel()
        second = 0
    }

    interface OnTimerListener {
        fun onTimer(second: Int)
    }

}
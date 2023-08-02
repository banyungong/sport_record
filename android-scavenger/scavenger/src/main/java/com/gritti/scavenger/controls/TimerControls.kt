package com.gritti.scavenger.controls

import java.util.Timer
import java.util.TimerTask

class TimerControls {

    private var second = 0
    private val mTimer: Timer by lazy {
        Timer()
    }

    private var mListeners = mutableListOf<onTimerListener>()

    fun startTimer() {
        mTimer.schedule(object : TimerTask() {
            override fun run() {
                second++
                mListeners.forEach {
                    it.onTimer(second)
                }
            }
        }, 0, 995)
    }

    fun addListener(listener: onTimerListener) {
        mListeners.add(listener)
    }

    fun removeListener(listener: onTimerListener) {
        mListeners.remove(listener)
    }

    fun stopTimer() {
        mTimer.cancel()
        second = 0
    }

    interface onTimerListener {
        fun onTimer(second: Int)
    }

}
package com.gritti.scavenger

import com.gritti.scavenger.model.Point
import com.gritti.scavenger.model.SportRecord

open class RecordServiceInterface {
    external fun setPath(dir: String, tag: String)

    external fun reset(): Int

    external fun init(frequency: Int): Int

    external fun start(): Int

    external fun resume(): Int

    external fun pause(): Int

    external fun stop(): Int

    external fun addPoint(
        second: Int,
        meter: Int,
        pace: Int,
        step: Int,
        altitude: Int,
        latitude: Int,
        longitude: Int,
        calorie: Int,
        climb: Int,
        type: Int )

    external fun getRecord(): SportRecord

    external fun getPointList(): ArrayList<Point>

    companion object {
        init {
            System.loadLibrary("scavenger")
        }
    }

}
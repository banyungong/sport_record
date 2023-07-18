package com.gritti.scavenger

import android.content.Context
import android.util.Log
import com.gritti.scavenger.model.Point
import com.gritti.scavenger.seneor.GyroscopeSensorHandler
import com.gritti.scavenger.seneor.StepSensorHandler

class ScavengerManager private constructor(context: Context) : ScavengerNative() {

    companion object {
        @Volatile
        private var instance: ScavengerManager? = null

        fun getInstance(context: Context): ScavengerManager {
            if (instance == null) {
                synchronized(ScavengerManager::class.java) {
                    if (instance == null) {
                        instance = ScavengerManager(context)
                    }
                }
            }
            return instance!!
        }
    }

    private var stepSensorHandler: StepSensorHandler? = null
    private var gyroscopeSensorHandler: GyroscopeSensorHandler? = null

    init {
        stepSensorHandler = StepSensorHandler(context)
        gyroscopeSensorHandler = GyroscopeSensorHandler(context)
    }

    fun start() {
        stepSensorHandler?.reset()
        gyroscopeSensorHandler?.reset()

        stepSensorHandler?.register()
        gyroscopeSensorHandler?.start()
    }

    fun stop() {
        stepSensorHandler?.unRegister()
        gyroscopeSensorHandler?.stop()
        clean()
    }

    fun filter(timestamp: Long, latitude: Double, longitude: Double): Point {
        Log.e("liruopeng", "timestamp: $timestamp, latitude: $latitude, longitude: $longitude")
        val result = action(
            timestamp,
            (latitude * 1000000).toInt(),
            (longitude * 1000000).toInt(),
            stepSensorHandler?.getStepCount() ?: 0,
            gyroscopeSensorHandler?.getValues() ?: arrayOf()
        )

        return Point(result[0], result[1] / 1000000f, result[2] / 1000000f, result[3].toInt())
    }


}
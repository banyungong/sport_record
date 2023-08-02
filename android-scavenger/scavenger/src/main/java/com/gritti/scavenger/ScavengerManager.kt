package com.gritti.scavenger

import android.content.Context
import android.location.Location
import android.location.LocationListener
import com.gritti.scavenger.model.Point
import com.gritti.scavenger.collection.GyroscopeSensorHandler
import com.gritti.scavenger.collection.LocationHandler
import com.gritti.scavenger.collection.StepSensorHandler

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
    private var locationHandler: LocationHandler? = null
    private var trackSmoothIntensity = 3
    private var antiCheatingIntensity = 3
    private var autoLocation = true

    init {
        stepSensorHandler = StepSensorHandler(context)
        gyroscopeSensorHandler = GyroscopeSensorHandler(context)
        locationHandler = LocationHandler(context)
    }

    fun start() {
        stepSensorHandler?.reset()
        gyroscopeSensorHandler?.reset()
        stepSensorHandler?.register()
        gyroscopeSensorHandler?.start()
        if (autoLocation) {
            locationHandler?.startLocation()
        }
        //获取内部存储路径
        init(trackSmoothIntensity, antiCheatingIntensity)
    }

    fun start(autoLocation: Boolean) {
        this.autoLocation = autoLocation
        start()
    }

    fun start(
        trackSmoothIntensity: Int,
        antiCheatingIntensity: Int,
        autoLocation: Boolean = true,
    ) {
        this.autoLocation = autoLocation
        this.trackSmoothIntensity = trackSmoothIntensity
        this.antiCheatingIntensity = antiCheatingIntensity
        start()
    }

    fun stop() {
        stepSensorHandler?.unRegister()
        gyroscopeSensorHandler?.stop()
        if (autoLocation) {
            locationHandler?.stopLocation()
        }
        locationHandler?.stopLocation()
        clean()
    }


    fun filter(timestamp: Long, latitude: Double, longitude: Double): Point {
        val result = action(
            timestamp,
            (latitude * 1000000).toInt(),
            (longitude * 1000000).toInt(),
            stepSensorHandler?.getStepCount() ?: 0,
            gyroscopeSensorHandler?.getValues() ?: arrayOf()
        )

        return Point(
            result[0],
            result[1] / 1000000f,
            result[2] / 1000000f,
            result[3].toInt(),
            result[4].toInt(),
            result[5].toInt(),
            result[6].toInt(),
            result[7] / 1000000f,
            result[8].toInt(),
            result[9].toInt()
        )
    }

    fun filter(): Point {
        return filter(
            System.currentTimeMillis() / 1000,
            locationHandler?.lastLocation?.latitude ?: 0.0,
            locationHandler?.lastLocation?.longitude ?: 0.0
        )
    }

    fun pointRarefy(latlngs: Array<Point>): Array<Point> {
        val result = pointRarefy(latlngs.map {
            intArrayOf(
                it.timestamp.toInt(),
                (it.latitude * 1000000).toInt(),
                (it.longitude * 1000000).toInt(),
                it.type
            )
        }.toTypedArray())
        return result.map {
            Point(
                it[0].toLong(),
                it[1] / 1000000f,
                it[2] / 1000000f,
                it[3],
                it[4],
                it[5],
                it[6],
                it[7] / 1000000f,
                it[8],
                it[9]
            )
        }.toTypedArray()
    }

}
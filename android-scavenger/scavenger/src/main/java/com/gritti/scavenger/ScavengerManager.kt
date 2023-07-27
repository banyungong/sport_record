package com.gritti.scavenger

import android.content.Context
import com.gritti.scavenger.model.Point
import com.gritti.scavenger.seneor.GyroscopeSensorHandler
import com.gritti.scavenger.seneor.StepSensorHandler
import java.io.File

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
    private var defaultDir: String
    private var defaultFileName = "scavenger.json"
    private var trackSmoothIntensity = 3
    private var antiCheatingIntensity = 3

    init {
        defaultDir =
            context.getExternalFilesDir(null)?.absolutePath + File.separator + "scavenger" + File.separator
        stepSensorHandler = StepSensorHandler(context)
        gyroscopeSensorHandler = GyroscopeSensorHandler(context)
    }

    fun start() {
        stepSensorHandler?.reset()
        gyroscopeSensorHandler?.reset()
        stepSensorHandler?.register()
        gyroscopeSensorHandler?.start()
        //获取内部存储路径
        init(defaultDir, defaultFileName, trackSmoothIntensity, antiCheatingIntensity)
    }

    fun start(
        dir: String,
        fileName: String,
        trackSmoothIntensity: Int,
        antiCheatingIntensity: Int
    ) {
        this.defaultDir = dir
        this.defaultFileName = fileName
        this.trackSmoothIntensity = trackSmoothIntensity
        this.antiCheatingIntensity = antiCheatingIntensity
        start()
    }

    fun stop() {
        stepSensorHandler?.unRegister()
        gyroscopeSensorHandler?.stop()
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
            result[4].toInt()
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
            Point(it[0].toLong(), it[1] / 1000000f, it[2] / 1000000f, 0, 0)
        }.toTypedArray()
    }


}
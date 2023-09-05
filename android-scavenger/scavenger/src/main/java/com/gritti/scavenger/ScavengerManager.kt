package com.gritti.scavenger

import android.content.Context
import android.location.Location
import android.location.LocationListener
import com.gritti.scavenger.model.Point
import com.gritti.scavenger.collection.GyroscopeSensorHandler
import com.gritti.scavenger.collection.LocationHandler
import com.gritti.scavenger.collection.StepSensorHandler
import com.gritti.scavenger.controls.IRecordControls
import com.gritti.scavenger.controls.IRecordLifecycle
import com.gritti.scavenger.model.Option
import com.gritti.scavenger.model.SportRecord
import java.io.File

class ScavengerManager private constructor(context: Context) : IRecordControls {
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
    private var option: Option = Option()
    private var sportRecord = SportRecord()
    private val scavengerNative = ScavengerNative()
    private val applicationContext = context
    private var bindLifecycle = mutableListOf<IRecordLifecycle>()

    init {
        stepSensorHandler = StepSensorHandler(context)
        gyroscopeSensorHandler = GyroscopeSensorHandler(context)
        locationHandler = LocationHandler(context)
        option.sportFileDir =
            context.getExternalFilesDir(null)?.absolutePath + File.separator + "scavenger" + File.separator
    }

    private fun startWork() {
        stepSensorHandler?.reset()
        gyroscopeSensorHandler?.reset()
        stepSensorHandler?.register()
        gyroscopeSensorHandler?.start()
        if (option.autoLocation) {
            locationHandler?.startLocation()
        }
    }

    private fun stopWork() {
        stepSensorHandler?.unRegister()
        gyroscopeSensorHandler?.stop()
        if (option.autoLocation) {
            locationHandler?.stopLocation()
        }
        locationHandler?.stopLocation()
    }


    fun addPoint(timestamp: Long, latitude: Double, longitude: Double): Point {
        val result = scavengerNative.addPoint(
            timestamp,
            (latitude * 1000000).toInt(),
            (longitude * 1000000).toInt(),
            stepSensorHandler?.getStepCount() ?: 0,
            gyroscopeSensorHandler?.getValues() ?: arrayOf()
        )

        var point = Point(
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
        bindLifecycle.forEach {
            it.onUpdateRecord(sportRecord)
        }
        return point
    }

    fun addPoint(): Point {
        return addPoint(
            System.currentTimeMillis() / 1000,
            locationHandler?.lastLocation?.latitude ?: 0.0,
            locationHandler?.lastLocation?.longitude ?: 0.0
        )
    }


    override fun config(option: Option) {
        scavengerNative.config(
            option.trackSmoothIntensity,
            option.antiCheatingIntensity,
            option.sportFileDir,
            option.sportTag
        )
        sportRecord = scavengerNative.getRecord()
        if (sportRecord.status == 2) {
            sportRecord.pointList = scavengerNative.getPointList()
            resumeRecord()
        }
    }

    override fun resetRecord() {
        scavengerNative.sportReset()
    }

    override fun startRecord() {
        //开启服务
        if (scavengerNative.sportStart() >= 0) {
            RunningService.start(applicationContext)
            startWork()
            bindLifecycle.forEach {
                it.onStart()
            }
        }
    }

    override fun pauseRecord() {
        if (scavengerNative.sportPause() >= 0) {
            stopWork()
            bindLifecycle.forEach {
                it.onPause()
            }
        }
    }

    override fun resumeRecord() {
        if (scavengerNative.sportResume() >= 0) {
            startWork()
            bindLifecycle.forEach {
                it.onResume()
            }
        }
    }

    override fun stopRecord() {
        if (scavengerNative.sportStop() >= 0) {
            stopWork()
            RunningService.stop(applicationContext)
            bindLifecycle.forEach {
                it.onStop()
            }
        }
    }

    override fun registerLifecycle(lifecycle: IRecordLifecycle) {
        bindLifecycle.add(lifecycle)
    }

    override fun unRegisterLifecycle(lifecycle: IRecordLifecycle) {
        bindLifecycle.remove(lifecycle)
    }

    fun getSportRecord(): SportRecord {
        return sportRecord
    }

    fun pointRarefy(latlngs: Array<Point>): Array<Point> {
        val result = scavengerNative.pointRarefy(latlngs.map {
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
package com.gritti.scavenger

import android.Manifest
import android.content.Context
import android.content.pm.PackageManager
import android.os.Build
import android.util.Log
import androidx.core.content.ContextCompat
import com.gritti.scavenger.model.Point
import com.gritti.scavenger.collection.GyroscopeSensorHandler
import com.gritti.scavenger.collection.LocationHandler
import com.gritti.scavenger.collection.StepSensorHandler
import com.gritti.scavenger.controls.IRecordControls
import com.gritti.scavenger.controls.IRecordLifecycle
import com.gritti.scavenger.controls.TimerControls
import com.gritti.scavenger.mock.MockLocation
import com.gritti.scavenger.mock.MockManager
import com.gritti.scavenger.model.Option
import com.gritti.scavenger.model.SportRecord
import com.gritti.scavenger.model.copySimpleRecord
import com.gritti.scavenger.model.isPause
import com.gritti.scavenger.model.isRunning
import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.newSingleThreadContext
import kotlinx.coroutines.withContext
import java.io.File

class ScavengerManager private constructor(context: Context) : IRecordControls,
    TimerControls.OnTimerListener {
    companion object {
        @Volatile
        private var instance: IRecordControls? = null
        fun getInstance(context: Context): IRecordControls {
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

    private val timerControls: TimerControls by lazy {
        TimerControls()
    }

    private var stepSensorHandler: StepSensorHandler? = null
    private var gyroscopeSensorHandler: GyroscopeSensorHandler? = null
    private var locationHandler: LocationHandler? = null
    private var option: Option = Option()
    private var sportRecord = SportRecord()
    private val scavengerNative = ScavengerNative()
    private val applicationContext = context
    private var bindLifecycle = mutableListOf<IRecordLifecycle>()

    private val mockManager by lazy {
        MockManager(context)
    }

    @OptIn(DelicateCoroutinesApi::class)
    private val singleThreadContext by lazy {
        newSingleThreadContext("scavenger")
    }

    private suspend fun <T> withSingleThreadContext(block: suspend () -> T): T {
        return withContext(singleThreadContext) {
            block()
        }
    }

    init {
        stepSensorHandler = StepSensorHandler(context)
        gyroscopeSensorHandler = GyroscopeSensorHandler(context)
        locationHandler = LocationHandler(context)
        option.sportFileDir =
            context.filesDir?.absolutePath + File.separator + "scavenger" + File.separator
        mockManager.open = true
        config()
    }

    private fun startWork() {
        timerControls.startTimer()
        timerControls.addListener(this)
        stepSensorHandler?.reset()
        gyroscopeSensorHandler?.reset()
        stepSensorHandler?.register()
        gyroscopeSensorHandler?.start()
        if (option.autoLocation) {
            locationHandler?.startLocation()
        }
    }

    private fun stopWork() {
        timerControls.stopTimer()
        timerControls.removeListener(this)
        stepSensorHandler?.unRegister()
        gyroscopeSensorHandler?.stop()
        if (option.autoLocation) {
            locationHandler?.stopLocation()
        }
        locationHandler?.stopLocation()
    }

    private fun addPoint(timestamp: Long, latitude: Double, longitude: Double): Point {
        val result = if (mockManager.open) {
            val mockData = mockManager.getMockData(sportRecord.duration + 1)
            Log.e(
                "liruopeng",
                "timestamp:${mockData.timestamp} , latitude:${mockData.latitude} , longitude:${mockData.longitude} , step:${mockData.step}"
            )
            scavengerNative.addPoint(
                mockData.timestamp,
                (mockData.latitude * 1000000).toInt(),
                (mockData.longitude * 1000000).toInt(),
                mockData.step,
                arrayOf()
            )
        } else {
            Log.e("liruopeng", "timestamp:$timestamp , latitude:$latitude , longitude:$longitude")
            scavengerNative.addPoint(
                timestamp,
                (latitude * 1000000).toInt(),
                (longitude * 1000000).toInt(),
                stepSensorHandler?.getStepCount() ?: 0,
                gyroscopeSensorHandler?.getValues() ?: arrayOf()
            )
        }
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

    private fun action() {
        val point = addPoint(
            System.currentTimeMillis() / 1000,
            locationHandler?.lastLocation?.latitude ?: 0.0,
            locationHandler?.lastLocation?.longitude ?: 0.0
        )
        Log.e("liruopeng", "meter:${point.meter} , type;${point.type}")
        updateSimpleSportRecord()
    }

    private fun updateSimpleSportRecord() {
        sportRecord.copySimpleRecord(scavengerNative.getSimpleRecord())
        bindLifecycle.forEach {
            it.onUpdateRecord(sportRecord)
        }
    }


    private fun config() {
        scavengerNative.config(
            this@ScavengerManager.option.trackSmoothIntensity,
            this@ScavengerManager.option.antiCheatingIntensity,
            this@ScavengerManager.option.sportFileDir,
        )
        updateSimpleSportRecord()
        if (sportRecord.isRunning() || sportRecord.isPause()) {
            RunningService.start(applicationContext)
            if (sportRecord.isRunning())
                startWork()
        }
    }

    override suspend fun resetRecord(): Boolean {
        return withSingleThreadContext {
            scavengerNative.sportReset()
            mockManager.reset()
            true
        }
    }

    override suspend fun startRecord(): Boolean {
        return withSingleThreadContext {
            if (ContextCompat.checkSelfPermission(
                    applicationContext,
                    Manifest.permission.ACCESS_FINE_LOCATION
                ) != PackageManager.PERMISSION_GRANTED
            ) {
                return@withSingleThreadContext false
            }
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q && ContextCompat.checkSelfPermission(
                    applicationContext,
                    Manifest.permission.ACTIVITY_RECOGNITION
                ) != PackageManager.PERMISSION_GRANTED
            ) {
                return@withSingleThreadContext false
            }

            //开启服务
            if (scavengerNative.sportStart() >= 0) {
                RunningService.start(applicationContext)
                startWork()
                return@withSingleThreadContext true
            }
            return@withSingleThreadContext false
        }
    }

    override suspend fun pauseRecord(): Boolean {
        return withSingleThreadContext {
            if (scavengerNative.sportPause() >= 0) {
                stopWork()
                updateSportRecord()
                return@withSingleThreadContext true
            }
            false
        }

    }

    override suspend fun resumeRecord(): Boolean {
        return withSingleThreadContext {
            if (scavengerNative.sportResume() >= 0) {
                startWork()
                updateSimpleSportRecord()
                return@withSingleThreadContext true
            }
            false
        }

    }

    override suspend fun stopRecord(): Boolean {
        return withSingleThreadContext {
            if (scavengerNative.sportStop() >= 0) {
                stopWork()
                RunningService.stop(applicationContext)
                // copy数据
                return@withSingleThreadContext true
            }
            false
        }

    }

    override fun registerLifecycle(lifecycle: IRecordLifecycle) {
        bindLifecycle.add(lifecycle)
    }

    override fun unRegisterLifecycle(lifecycle: IRecordLifecycle) {
        bindLifecycle.remove(lifecycle)
    }

    override fun getSportRecord(): SportRecord {
        return sportRecord
    }

    override suspend fun updateSportRecord(): SportRecord {
        return withSingleThreadContext {
            sportRecord = scavengerNative.getRecord()
            bindLifecycle.forEach {
                it.onUpdateRecord(sportRecord)
            }
            sportRecord
        }
    }

    override suspend fun pointRarefy(latlngs: Array<Point>): Array<Point> {
        return withSingleThreadContext {
            val result = scavengerNative.pointRarefy(latlngs.map {
                intArrayOf(
                    it.timestamp.toInt(),
                    (it.latitude * 1000000).toInt(),
                    (it.longitude * 1000000).toInt(),
                    it.type
                )
            }.toTypedArray())
            result.map {
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

    override fun onTimer(second: Int) {
        action()
    }
}
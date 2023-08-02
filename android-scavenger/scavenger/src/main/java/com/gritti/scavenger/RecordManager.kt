package com.gritti.scavenger

import android.content.Context
import com.gritti.scavenger.model.SportRecord
import java.io.File

class RecordManager private constructor(context: Context) : RecordServiceInterface() {
    companion object {
        @Volatile
        private var instance: RecordManager? = null
        fun getInstance(context: Context): RecordManager {
            if (instance == null) {
                synchronized(ScavengerManager::class.java) {
                    if (instance == null) {
                        instance = RecordManager(context)
                    }
                }
            }
            return instance!!
        }
    }

    private var applicationContext: Context
    private var defaultDir: String = ""
    private var defaultFileName = "scavenger"
    private var sportRecord = SportRecord()
    private var mListeners = mutableListOf<OnRecordListener>()

    init {
        applicationContext = context.applicationContext
        defaultDir =
            context.getExternalFilesDir(null)?.absolutePath + File.separator + "scavenger" + File.separator
        setPath(defaultDir, defaultFileName)
        sportRecord = getRecord()
        if (sportRecord.status == 2) {
            sportRecord.pointList = getPointList()
            resumeRecord()
        }
    }

    fun addListener(listener: OnRecordListener) {
        mListeners.add(listener)
    }

    fun removeListener(listener: OnRecordListener) {
        mListeners.remove(listener)
    }

    fun notifyRecord() {
        mListeners.forEach {
            it.onUpdateRecord(sportRecord)
        }
    }

    fun getSportRecord(): SportRecord {
        return sportRecord
    }

    fun configRecord(frequency: Int) {
        if (reset() >= 0) {
            init(frequency)
        }
    }

    fun startRecord() {
        //开启服务
        if (start() >= 0) {
            RunningService.start(applicationContext)
            ScavengerManager.getInstance(applicationContext).start(true)
        }
    }

    fun pauseRecord() {
        if (pause() >= 0) {
            ScavengerManager.getInstance(applicationContext).stop()
        }
    }

    fun resumeRecord() {
        if (resume() >= 0) {
            ScavengerManager.getInstance(applicationContext).start(true)
        }
    }

    fun stopRecord() {
        if (stop() >= 0) {
            RunningService.stop(applicationContext)
            ScavengerManager.getInstance(applicationContext).stop()
        }
    }

    interface OnRecordListener {
        fun onUpdateRecord(record: SportRecord)
    }

}
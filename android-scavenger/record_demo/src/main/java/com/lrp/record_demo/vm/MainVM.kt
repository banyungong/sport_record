package com.lrp.record_demo.vm

import android.app.Application
import android.content.Intent
import androidx.core.view.ContentInfoCompat.Flags
import com.gritti.scavenger.ScavengerManager
import com.gritti.scavenger.model.isPause
import com.gritti.scavenger.model.isRunning
import com.lrp.record_demo.RecordActivity
import com.lrp.record_demo.base.BaseVM

class MainVM(application: Application) : BaseVM(application) {
    private val scavengerManager by lazy {
        ScavengerManager.getInstance(application)
    }


    fun checkRecordStatus(){
        runMainThread {
            val sportRecord = scavengerManager.config(null)
            if (sportRecord.isRunning() || sportRecord.isPause()) {
                val intent = Intent(getApplication(), RecordActivity::class.java)
                intent.flags = Intent.FLAG_ACTIVITY_NEW_TASK

                getApplication<Application>().startActivity(intent)
            }
        }
    }

    fun startRecord() {
        runMainThread {
            if (scavengerManager.resetRecord()) {
                scavengerManager.startRecord()
                val intent = Intent(getApplication(), RecordActivity::class.java)
                intent.flags = Intent.FLAG_ACTIVITY_NEW_TASK
                getApplication<Application>().startActivity(intent)
            }
        }
    }
}
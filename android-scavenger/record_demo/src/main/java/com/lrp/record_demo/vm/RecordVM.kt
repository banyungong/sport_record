package com.lrp.record_demo.vm

import android.app.Application
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.viewModelScope
import com.gritti.scavenger.ScavengerManager
import com.gritti.scavenger.controls.IRecordLifecycle
import com.gritti.scavenger.model.SportRecord
import com.gritti.scavenger.model.isPause
import com.gritti.scavenger.model.isRunning
import com.lrp.record_demo.base.BaseVM
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.async
import kotlinx.coroutines.launch
import kotlinx.coroutines.runBlocking
import kotlinx.coroutines.withContext

class RecordVM(application: Application) : BaseVM(application), IRecordLifecycle {
    val sportRecordLV = MutableLiveData<SportRecord>()
    val pauseOrContinueLV = MutableLiveData<String>()

    private val scavengerManager by lazy {
        ScavengerManager.getInstance(application)
    }

    init {
        scavengerManager.registerLifecycle(this)
        sportRecordLV.value = scavengerManager.getSportRecord()
        sportRecordLV.value?.let {
            if (it.isRunning()) {
                pauseOrContinueLV.value = "暂停"
            } else if (it.isPause()) {
                pauseOrContinueLV.value = "继续"
            }
        }
    }


    override fun onCleared() {
        super.onCleared()
        scavengerManager.unRegisterLifecycle(this)
    }

    fun pauseOrContinue() {
        runMainThread {
            sportRecordLV.value?.let {
                if (it.isRunning()) {
                    if (scavengerManager.pauseRecord())
                        pauseOrContinueLV.postValue("继续")
                } else if (it.isPause()) {
                    if (scavengerManager.resumeRecord())
                        pauseOrContinueLV.postValue("暂停")
                }
            }
        }
    }

    fun finishRecord() {
        runMainThread {
            if (scavengerManager.stopRecord()) {
                finishRecord()
            }
        }
    }

    override fun onUpdateRecord(record: SportRecord) {
        runMainThread {
            sportRecordLV.postValue(record)
        }
    }


}
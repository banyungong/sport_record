package com.lrp.record_demo.vm

import android.app.Application
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.MutableLiveData
import com.gritti.scavenger.ScavengerManager
import com.gritti.scavenger.controls.IRecordLifecycle
import com.gritti.scavenger.model.SportRecord

class RecordVM(application: Application) : AndroidViewModel(application), IRecordLifecycle {
    val distanceMutableLiveData = MutableLiveData<String>()
    val secondMutableLiveData = MutableLiveData<String>()
    val speedMutableLiveData = MutableLiveData<String>()
    val calorieMutableLiveData = MutableLiveData<String>()
    val climbMutableLiveData = MutableLiveData<String>()
    val stepMutableLiveData = MutableLiveData<String>()
    val stepFrequencyMutableLiveData = MutableLiveData<String>()

    init {
        distanceMutableLiveData.value = "0.00KM"
        secondMutableLiveData.value = "00:00:00"
        speedMutableLiveData.value = "0'00\""
        calorieMutableLiveData.value = "0Cal"
        climbMutableLiveData.value = "0m"
        stepMutableLiveData.value = "0Step"
        stepFrequencyMutableLiveData.value = "0/min"
        ScavengerManager.getInstance(application).registerLifecycle(this)
    }

    override fun onCleared() {
        super.onCleared()
        ScavengerManager.getInstance(getApplication()).unRegisterLifecycle(this)
    }

    override fun onStart() {

    }

    override fun onPause() {
        TODO("Not yet implemented")
    }

    override fun onResume() {
        TODO("Not yet implemented")
    }

    override fun onStop() {
        TODO("Not yet implemented")
    }

    override fun onUpdateRecord(record: SportRecord) {
        distanceMutableLiveData.value = String.format("%.2fKM", record.mileage)
        secondMutableLiveData.value = String.format(
            "%02d:%02d:%02d",
            record.duration / 3600,
            record.duration % 3600 / 60,
            record.duration % 60
        )
        speedMutableLiveData.value = String.format(
            "%d'%02d\"",
            record.speed / 60,
            record.speed % 60
        )
        calorieMutableLiveData.value = String.format("%dCal", record.calorie)
        climbMutableLiveData.value = String.format("%dm", record.climb)
        stepMutableLiveData.value = String.format("%dStep", record.step)
        stepFrequencyMutableLiveData.value = String.format("%d/min", record.step)
    }


}
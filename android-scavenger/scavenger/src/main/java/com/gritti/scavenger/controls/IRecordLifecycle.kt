package com.gritti.scavenger.controls

import com.gritti.scavenger.model.SportRecord

interface IRecordLifecycle {

    fun onStart()

    fun onPause()

    fun onResume()

    fun onStop()

    fun onUpdateRecord(record: SportRecord)

}
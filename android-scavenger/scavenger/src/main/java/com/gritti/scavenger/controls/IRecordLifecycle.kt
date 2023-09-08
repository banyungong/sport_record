package com.gritti.scavenger.controls

import com.gritti.scavenger.model.SportRecord

interface IRecordLifecycle {
    fun onUpdateRecord(record: SportRecord)
}
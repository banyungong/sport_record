package com.gritti.scavenger.controls

import com.gritti.scavenger.model.Option

interface IRecordControls {
    fun config(option: Option)

    fun resetRecord()

    fun startRecord()

    fun pauseRecord()

    fun resumeRecord()

    fun stopRecord()

    fun registerLifecycle(lifecycle: IRecordLifecycle)

    fun unRegisterLifecycle(lifecycle: IRecordLifecycle)

}
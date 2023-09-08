package com.gritti.scavenger.controls

import com.gritti.scavenger.model.Option
import com.gritti.scavenger.model.Point
import com.gritti.scavenger.model.SportRecord

interface IRecordControls {
    suspend fun config(option: Option?): SportRecord

    suspend fun resetRecord(): Boolean

    suspend fun startRecord(): Boolean

    suspend fun pauseRecord(): Boolean

    suspend fun resumeRecord(): Boolean

    suspend fun stopRecord(): Boolean

    fun getSportRecord(): SportRecord

    suspend fun pointRarefy(latlngs: Array<Point>): Array<Point>

    fun registerLifecycle(lifecycle: IRecordLifecycle)

    fun unRegisterLifecycle(lifecycle: IRecordLifecycle)

}
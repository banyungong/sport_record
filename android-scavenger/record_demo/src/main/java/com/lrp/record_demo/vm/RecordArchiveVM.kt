package com.lrp.record_demo.vm

import android.app.Application
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.viewModelScope
import com.amap.api.maps.CoordinateConverter
import com.amap.api.maps.model.LatLng
import com.gritti.scavenger.ScavengerManager
import com.gritti.scavenger.model.SportRecord
import com.lrp.record_demo.base.BaseVM
import kotlinx.coroutines.launch

class RecordArchiveVM(application: Application) : BaseVM(application) {
    private val scavengerManager by lazy {
        ScavengerManager.getInstance(application)
    }

    private var record: SportRecord = scavengerManager.getSportRecord()
    val latLngs: MutableLiveData<List<LatLng>> = MutableLiveData()

    init {
        viewModelScope.launch {
            record=scavengerManager.updateSportRecord()
            parseRecord()
        }
    }

    private fun parseRecord() {
        //归类出坐标点集合
        latLngs.value = record.pointList.map { point ->
            val converter = CoordinateConverter(getApplication())
            converter.from(CoordinateConverter.CoordType.GPS)
            converter.coord(
                LatLng(
                    point.latitude.toDouble(),
                    point.longitude.toDouble()
                )
            ).convert()
        }
        //归类出每公里配速
        //以里程为维度，归类出每100米配速集合

        //以时间为维度，归类出每5秒配速集合

        //以时间为维度，归类出每5秒步频集合

        //以时间为维度，归类出每5秒步幅集合

        //以里程为维度，归类出每100米的海拔集合
        //以时间为维度，归类出每5秒海拔集合

    }

}
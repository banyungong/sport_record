package com.lrp.record_demo

import android.graphics.Color
import android.os.Bundle
import com.amap.api.maps.AMap
import com.amap.api.maps.CameraUpdate
import com.amap.api.maps.CameraUpdateFactory
import com.amap.api.maps.model.CameraPosition
import com.amap.api.maps.model.LatLngBounds
import com.amap.api.maps.model.PolylineOptions
import com.lrp.record_demo.base.BaseActivity
import com.lrp.record_demo.databinding.ActivityRecordArchiveBinding
import com.lrp.record_demo.vm.RecordArchiveVM

class RecordArchiveActivity : BaseActivity<RecordArchiveVM, ActivityRecordArchiveBinding>() {
    override fun getClazz(): Class<RecordArchiveVM> {
        return RecordArchiveVM::class.java
    }

    override fun layout(): Int {
        return R.layout.activity_record_archive
    }

    override fun initView() {
        viewModel.latLngs.observe(this) {
            dataBinding?.apply {
                val polyline = PolylineOptions().addAll(it)
                    .width(16f)
                    .color(Color.RED)
                var latLngBounds = LatLngBounds(
                    it.first(),
                    it[1]
                )
                it.forEach { latlng ->
                    latLngBounds = latLngBounds.including(latlng)
                }
                mapView.map.addPolyline(polyline)
                mapView.map.animateCamera(
                    CameraUpdateFactory.newLatLngBounds(
                        latLngBounds, 50
                    )
                )
            }
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        dataBinding?.mapView?.onCreate(savedInstanceState)
    }

    override fun onResume() {
        super.onResume()
        dataBinding?.mapView?.onResume()
    }

    override fun onPause() {
        super.onPause()
        dataBinding?.mapView?.onPause()
    }

    override fun onDestroy() {
        super.onDestroy()
        dataBinding?.mapView?.onDestroy()
    }
}
package com.lrp.record_demo

import android.app.Application
import com.amap.api.location.AMapLocationClient
import com.amap.api.maps.MapsInitializer
import com.gritti.scavenger.ScavengerManager

class RecordApplication : Application() {
    override fun onCreate() {
        super.onCreate()
        MapsInitializer.updatePrivacyShow(this, true, true)
        MapsInitializer.updatePrivacyAgree(this, true)
        ScavengerManager.getInstance(this)
    }
}
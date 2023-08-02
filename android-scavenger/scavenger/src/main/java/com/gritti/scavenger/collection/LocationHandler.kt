package com.gritti.scavenger.collection

import android.Manifest
import android.content.Context
import android.content.pm.PackageManager
import android.location.Location
import android.location.LocationListener
import android.location.LocationManager
import androidx.core.app.ActivityCompat

class LocationHandler constructor(context: Context) : LocationListener {

    private var mLocationManager: LocationManager
    private var context: Context = context.applicationContext
    var lastLocation: Location? = null

    init {
        mLocationManager = context.getSystemService(Context.LOCATION_SERVICE) as LocationManager
    }

    fun startLocation() {
        if (ActivityCompat.checkSelfPermission(
                context,
                Manifest.permission.ACCESS_FINE_LOCATION
            ) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(
                context,
                Manifest.permission.ACCESS_COARSE_LOCATION
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            throw SecurityException("Location permission is not granted")
        }
        mLocationManager.requestLocationUpdates(
            LocationManager.GPS_PROVIDER,
            1000,
            0.5f,
            this
        )
    }

    fun stopLocation() {
        mLocationManager.removeUpdates(this)
    }

    override fun onLocationChanged(location: Location) {
        this.lastLocation = location
    }

}
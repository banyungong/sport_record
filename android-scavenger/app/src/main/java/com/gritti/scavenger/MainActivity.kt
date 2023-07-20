package com.gritti.scavenger

import android.Manifest
import android.content.pm.PackageManager
import android.graphics.Color
import android.location.Location
import android.location.LocationListener
import android.location.LocationManager
import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import androidx.activity.ComponentActivity
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.core.app.ActivityCompat
import com.amap.api.maps.CoordinateConverter
import com.amap.api.maps.MapView
import com.amap.api.maps.model.LatLng
import com.amap.api.maps.model.PolylineOptions
import com.gritti.scavenger.mock.MockLocation
import com.gritti.scavenger.ui.theme.ScavengerTheme
import java.util.Timer
import java.util.TimerTask
import com.gritti.scavenger.R
import com.gritti.scavenger.model.Point

class MainActivity : ComponentActivity(), LocationListener {

    private var mLocationManager: LocationManager? = null
    private var timer: Timer? = null
    private var second: Int = 0
    private var mapView: MapView? = null
    private var tvLocation: TextView? = null
    private var btnStart: Button? = null
    private var btnAutoStart: Button? = null
    private var btnStop: Button? = null
    private var orgLatLngs = mutableListOf<LatLng>()
    private var newLatLngs = mutableListOf<LatLng>()
    private var intervalLatLngs = mutableListOf<LatLng>()

    private var tempLatLngs = mutableListOf<Point>()
    private var rarefyLatLngs = mutableListOf<LatLng>()


    private var isRunning = false


    private fun updateControl() {
        btnStart?.visibility = if (isRunning) android.view.View.GONE else android.view.View.VISIBLE
        btnAutoStart?.visibility =
            if (isRunning) android.view.View.GONE else android.view.View.VISIBLE
        btnStop?.visibility = if (isRunning) android.view.View.VISIBLE else android.view.View.GONE
    }

    private fun start() {
        if (ActivityCompat.checkSelfPermission(
                applicationContext,
                Manifest.permission.ACCESS_FINE_LOCATION
            ) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(
                applicationContext,
                Manifest.permission.ACCESS_COARSE_LOCATION
            ) != PackageManager.PERMISSION_GRANTED
        ) {

            ActivityCompat.requestPermissions(
                this,
                arrayOf(
                    Manifest.permission.ACCESS_FINE_LOCATION,
                    Manifest.permission.ACCESS_COARSE_LOCATION
                ),
                1
            )
            return
        }
        isRunning = true
        orgLatLngs = mutableListOf()
        newLatLngs = mutableListOf()
        second = 0

        mLocationManager = getSystemService(LOCATION_SERVICE) as LocationManager
        mLocationManager?.requestLocationUpdates(
            LocationManager.GPS_PROVIDER,
            1000,
            1f,
            this@MainActivity
        )

        updateControl()
    }


    private fun autoStart() {
        isRunning = true
        val mockLocation = MockLocation(this)
        ScavengerManager.getInstance(this.applicationContext).start()
        timer = Timer()
        timer?.schedule(
            object : TimerTask() {
                override fun run() {
                    val location = mockLocation.getLocation(++second)
                    orgLatLngs.add(
                        CoordinateConverter(applicationContext).from(CoordinateConverter.CoordType.GPS)
                            .coord(LatLng(location.latitude, location.longitude)).convert()
                    )
                    val filter =
                        ScavengerManager.getInstance(this@MainActivity.applicationContext).filter(
                            second.toLong(),
                            location.latitude,
                            location.longitude,
                        )
                    newLatLngs.add(
                        CoordinateConverter(applicationContext).from(CoordinateConverter.CoordType.GPS)
                            .coord(LatLng(filter.latitude.toDouble(), filter.longitude.toDouble()))
                            .convert()
                    )
                    tempLatLngs.add(
                        filter
                    )
                    runOnUiThread {
                        tvLocation?.text = "作弊类别：${filter.type}"
                    }
                    if (second % 5 == 1) {
                        ScavengerManager.getInstance(applicationContext)
                            .pointRarefy(tempLatLngs.toTypedArray()).forEach {
                                rarefyLatLngs.add(
                                    CoordinateConverter(applicationContext).from(CoordinateConverter.CoordType.GPS)
                                        .coord(
                                            LatLng(
                                                it.latitude.toDouble(),
                                                it.longitude.toDouble()
                                            )
                                        )
                                        .convert()
                                )
                            }
                        tempLatLngs.clear();

                        intervalLatLngs.add(
                            orgLatLngs.last()
                        )
                        drawLine()
                    }
                }
            },
            0,
            1000
        )
        updateControl()
    }


    private fun stop() {
        isRunning = false
        timer?.cancel()
        ScavengerManager.getInstance(this.applicationContext).stop()
        isMoveCamera = false
        mLocationManager?.removeUpdates { }
        updateControl()
    }


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        mapView = findViewById(R.id.map_view)
        tvLocation = findViewById(R.id.tv_location)
        mapView?.onCreate(savedInstanceState)
        btnStart = findViewById(R.id.start)
        btnAutoStart = findViewById(R.id.auto_start)
        btnStop = findViewById(R.id.stop)
        updateControl()
        btnStart?.setOnClickListener {
            start()
        }
        btnAutoStart?.setOnClickListener {
            autoStart()
        }
        btnStop?.setOnClickListener {
            stop()
        }

    }

    private var isMoveCamera = false
    fun drawLine() {
        mapView?.let {
            it.map.apply {
                clear()
                addPolyline(
                    PolylineOptions().addAll(orgLatLngs).color(Color.RED)
                        .width(16f)
                )
                addPolyline(
                    PolylineOptions().addAll(newLatLngs).color(Color.GREEN)
                        .width(16f)
                )
                if (intervalLatLngs.isNotEmpty()) {
                    addPolyline(
                        PolylineOptions().addAll(intervalLatLngs).color(Color.BLUE)
                            .width(16f)
                    )
                }

                if (rarefyLatLngs.isNotEmpty()) {
                    addPolyline(
                        PolylineOptions().addAll(rarefyLatLngs).color(Color.YELLOW)
                            .width(16f)
                    )
                }

                if (!isMoveCamera) {
                    isMoveCamera = true
                    moveCamera(
                        com.amap.api.maps.CameraUpdateFactory.newLatLngZoom(
                            newLatLngs.last(),
                            19f
                        )
                    )
                }

            }
        }
    }

    override fun onDestroy() {
        mapView?.onDestroy()
        super.onDestroy()
    }

    override fun onResume() {
        super.onResume()
        mapView?.onResume()
    }

    override fun onPause() {
        super.onPause()
        mapView?.onPause()
    }

    override fun onLocationChanged(location: Location) {
        if (!isRunning) {
            return
        }
        orgLatLngs.add(
            CoordinateConverter(applicationContext).from(CoordinateConverter.CoordType.GPS)
                .coord(LatLng(location.latitude, location.longitude)).convert()
        )
        val filter =
            ScavengerManager.getInstance(this@MainActivity.applicationContext).filter(
                System.currentTimeMillis() / 1000,
                location.latitude,
                location.longitude,
            )
        newLatLngs.add(
            CoordinateConverter(applicationContext).from(CoordinateConverter.CoordType.GPS)
                .coord(LatLng(filter.latitude.toDouble(), filter.longitude.toDouble()))
                .convert()
        )
        runOnUiThread {
            tvLocation?.text = "作弊类别：${filter.type}"
        }
        if (orgLatLngs.size % 5 == 1) {
            drawLine()
        }
    }

}


@Composable
fun Greeting(name: String, modifier: Modifier = Modifier) {

//    Column(modifier = modifier) {
//        Text(
//            text = "text!",
//
//            )
//        Text(
//            text = "bay $name!",
//        )
//
//    }

    Text(
        text = "Hello $name!",
        modifier = modifier
    )
}

@Preview(showBackground = true)

@Composable
fun GreetingPreview() {
    ScavengerTheme {
        Greeting("Android", Modifier.padding(all = 16.dp))
    }
}
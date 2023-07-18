package com.gritti.scavenger

import android.graphics.Color
import android.os.Bundle
import android.util.Log
import android.widget.TextView
import androidx.activity.ComponentActivity
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.amap.api.maps.CoordinateConverter
import com.amap.api.maps.MapView
import com.amap.api.maps.model.LatLng
import com.amap.api.maps.model.PolylineOptions
import com.gritti.scavenger.mock.MockLocation
import com.gritti.scavenger.ui.theme.ScavengerTheme
import java.util.Timer
import java.util.TimerTask

class MainActivity : ComponentActivity() {

    private lateinit var timer: Timer
    private var second: Int = 0
    private var mapView: MapView? = null
    private var tvLocation :TextView? = null

    private var orgLatLngs = mutableListOf<LatLng>()
    private var newLatLngs = mutableListOf<LatLng>()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(com.gritti.scavenger.R.layout.activity_main)
        mapView = findViewById(com.gritti.scavenger.R.id.map_view)
        tvLocation = findViewById(com.gritti.scavenger.R.id.tv_location)
        mapView?.onCreate(savedInstanceState)

        val mockLocation = MockLocation(this)
        ScavengerManager.getInstance(this.applicationContext).start()
        timer = Timer()
        timer.schedule(
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
                    runOnUiThread {
                            tvLocation?.text = "作弊类别：${filter.type}"
                    }
                    if (second % 5 == 1) {
                        drawLine()
                    }
                }
            },
            0,
            1000
        )
    }

    var isMoveCamera = false
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
        timer.cancel()
        ScavengerManager.getInstance(this.applicationContext).stop()
    }

    override fun onResume() {
        super.onResume()
        mapView?.onResume()
    }

    override fun onPause() {
        super.onPause()
        mapView?.onPause()
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
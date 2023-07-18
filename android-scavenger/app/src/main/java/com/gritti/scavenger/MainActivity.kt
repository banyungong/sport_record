package com.gritti.scavenger

import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.gritti.scavenger.mock.MockLocation
import com.gritti.scavenger.ui.theme.ScavengerTheme
import java.util.Timer
import java.util.TimerTask

class MainActivity : ComponentActivity() {

    private lateinit var timer: Timer
    private var second: Int = 0

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            ScavengerTheme {
                // A surface container using the 'background' color from the theme
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    Greeting("Android")
                }
            }
        }

        val mockLocation = MockLocation(this)
        ScavengerManager.getInstance(this.applicationContext).start()
        timer = Timer()
        timer.schedule(
            object : TimerTask() {
                override fun run() {
                    Log.e("liruopeng", "------------------------------------")
                    val location = mockLocation.getLocation(++second)
                    val filter =
                        ScavengerManager.getInstance(this@MainActivity.applicationContext).filter(
                            second.toLong(),
                            location.latitude,
                            location.longitude,
                        )
                    Log.e("liruopeng", "filter: $filter")
                    Log.e("liruopeng", "------------------------------------\n\n")
                }
            },
            0,
            1000
        )
    }

    override fun onDestroy() {
        super.onDestroy()
        timer.cancel()
        ScavengerManager.getInstance(this.applicationContext).stop()
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
package com.gritti.scavenger.mock

import android.content.Context
import java.util.Random
import kotlin.math.abs

class MockManager(context: Context) {

    var open = false


    private val mockLocation by lazy {
        MockLocation(context)
    }
    private var lastSecond: Int = 0

    private var step = 0


    fun getMockData(second: Int): MockData {
        val location = mockLocation.getLocation(second)
        step += abs(second - lastSecond) * Random().nextInt(4) + 1
        val mockData = MockData(
            System.currentTimeMillis() / 1000,
            location.latitude,
            location.longitude,
            step
        )
        lastSecond = second
        return mockData

    }

    fun reset() {
        lastSecond = 0
    }

}

data class MockData(val timestamp: Long, val latitude: Double, val longitude: Double, val step: Int)
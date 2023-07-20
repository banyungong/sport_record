package com.gritti.scavenger.mock

import android.content.Context
import android.location.Location
import com.alibaba.fastjson.JSON
import com.alibaba.fastjson.JSONArray
import java.io.IOException
import java.io.InputStream
import java.util.Random


class MockLocation(private var context: Context?) {
    private var data: MutableList<DoubleArray>? = null

    fun getLocation(second: Int): Location {
        val location = Location("gps")
        val tmp = getLocation1(second)
        location.latitude = tmp[0]
        location.longitude = tmp[1]
        location.time = System.currentTimeMillis()
        location.altitude = getRandom(1, 22)
        return location
    }

    private fun getRandom(min: Int, max: Int): Double {
        val random = Random()
        val value: Int = random.nextInt(max) % (max - min + 1) + min
        return value.toDouble()
    }


    private fun getLocation1(second: Int): DoubleArray {
        if (data == null) {
            data = mutableListOf()
            val text: String = read("mock_path2.json") ?: return doubleArrayOf(0.0, 0.0)
            val array: JSONArray = JSON.parseArray(text)
            for (i in 0 until array.size) {
                val it = array.getJSONArray(i)
                data?.add(
                    doubleArrayOf(
                        it.getFloatValue(0).toDouble(),
                        it.getFloatValue(1).toDouble()
                    )
                )
            }
        }
        val pace = 334.0
        val index = (second / (pace / 334)).toInt() % 4405
        return data!![index]
    }

    private fun read(fileName: String?): String? {
        if (fileName == null) return null
        var input: InputStream? = null
        try {
            input = context?.assets?.open(fileName)
            return input?.reader()?.readText()
        } catch (e: IOException) {
            e.printStackTrace()
        } finally {
            try {
                input?.close()
            } catch (e: IOException) {
                e.printStackTrace()
            }
        }
        return null
    }
}
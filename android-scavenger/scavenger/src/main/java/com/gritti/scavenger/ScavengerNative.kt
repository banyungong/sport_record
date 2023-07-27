package com.gritti.scavenger

open class ScavengerNative {

    external fun init(dir:String, filename:String, trackSmoothIntensity: Int,antiCheatingIntensity: Int)

    external fun action(
        timestamp: Long,
        latitude: Int,
        longitude: Int,
        stepCount: Int,
        gyroscope: Array<LongArray>
    ): LongArray

    external fun clean()

    external fun pointRarefy(latlngs: Array<IntArray>): Array<IntArray>

    companion object {
        init {
            System.loadLibrary("scavenger")
        }
    }
}
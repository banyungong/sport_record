package com.gritti.scavenger

open class ScavengerNative {
    external fun action(
        timestamp: Long,
        latitude: Int,
        longitude: Int,
        stepCount: Int,
        gyroscope: Array<LongArray>
    ): LongArray

    external fun clean()

    companion object {
        init {
            System.loadLibrary("scavenger")
        }
    }
}
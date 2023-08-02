package com.gritti.scavenger.model

class Point {
    var timestamp: Long = 0
    var latitude: Float = 0f
    var longitude: Float = 0f
    var pace: Int = 9999
    var step: Int = 0
    var calorie: Int = 0
    var climb: Int = 0
    var altitude: Float = 0f
    var type: Int = 0
    var meter: Int = 0

    constructor(
        timestamp: Long,
        latitude: Float,
        longitude: Float,
        pace: Int,
        step: Int,
        calorie: Int,
        climb: Int,
        altitude: Float,
        type: Int,
        meter: Int
    ) {
        this.timestamp = timestamp
        this.latitude = latitude
        this.longitude = longitude
        this.pace = pace
        this.step = step
        this.calorie = calorie
        this.climb = climb
        this.altitude = altitude
        this.type = type
        this.meter = meter
    }
}
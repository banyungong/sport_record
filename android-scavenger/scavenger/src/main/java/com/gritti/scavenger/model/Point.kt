package com.gritti.scavenger.model

class Point(
    var timestamp: Long,
    var latitude: Float,
    var longitude: Float,
    var pace: Int,
    var step: Int,
    var calorie: Int,
    var climb: Int,
    var altitude: Float,
    var type: Int,
    var meter: Int
)
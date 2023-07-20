//
// Created by liruopeng on 2023/6/25.
//

#include "sensor_value.h"

float SensorValue::getValue() const {
    return value;
}

unsigned long long SensorValue::getMillisecond() const {
    return millisecond;
}

unsigned int SensorValue::getWaveType() const {
    return waveType;
}

SensorValue::SensorValue(float value, unsigned long long millisecond, unsigned int waveType)
        : value(
        value), millisecond(millisecond), waveType(waveType) {}

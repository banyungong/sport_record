
//
// Created by liruopeng on 2023/6/25.
//

#ifndef SCAVENGER_SENSOR_VALUE_H
#define SCAVENGER_SENSOR_VALUE_H

///传感器数据实体
class SensorValue {
private:
    float value;
    unsigned long long millisecond;
    //0 表示未知，1 波峰 2波谷
    unsigned int waveType;
public:

    SensorValue(float value, unsigned long long millisecond, unsigned int waveType);

    float getValue() const;

    unsigned long long getMillisecond() const;

    unsigned int getWaveType() const;

};

#endif //SCAVENGER_SENSOR_VALUE_H
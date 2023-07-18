
//
// Created by liruopeng on 2023/6/25.
//

#ifndef SCAVENGER_CPOINT_H
#define SCAVENGER_CPOINT_H

#include "sensor_value.h"
#include <list>

enum CPoint_TYPE {
    TYPE_NORMAL = 0,
    //百米速度超过博尔特
    TYPE_PACE_ERROR1 = 20,
    //超过当前里程碑的世界记录
    TYPE_PACE_ERROR2 = 21,
    //两个最大速度之间的平均速度大于世界记录
    TYPE_PACE_ERROR3 = 22,
    //步幅超过极限
    TYPE_STEP_ERROR1 = 30,
    //传感器没有问题，步数为0，但是GPS的移动距离超过阀值
    TYPE_STEP_ERROR2 = 31,
    //没有步数警告
    TYPE_STEP_ERROR3 = 32,
    //步频超过极限
    TYPE_STEP_CADENCE = 40,
    //使用摇步器
    TYPE_SHAKE_CHEAT = 50
};

class CPoint {
private:

public:
    ~CPoint();
    long long timestamp;
    int latitude;
    int longitude;
    int step;
    CPoint_TYPE type;
    long long (*sensorValue)[4];
    int sensorSize;

};


#endif //SCAVENGER_CPOINT_H

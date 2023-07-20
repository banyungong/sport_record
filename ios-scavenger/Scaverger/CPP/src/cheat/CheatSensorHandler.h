//
// Created by liruopeng on 2022/12/9.
//

#ifndef SCAVENGER_CHEATSENSORHANDLER_H
#define SCAVENGER_CHEATSENSORHANDLER_H

#include <list>
#include "../model/sensor_value.h"

///传感器防止作弊
class CheatSensorHandler {
private:
    int time_offset = 10;
    float line_offset = 2;
    float rate_of_accuracy = 0.8;

    //X轴传感器数据
    std::list<SensorValue> *x_values = new std::list<SensorValue>;
    //Y轴传感器数据
    std::list<SensorValue> *y_values = new std::list<SensorValue>;
    //Z轴传感器数据
    std::list<SensorValue> *z_values = new std::list<SensorValue>;


    float x_last = 1000000;
    float y_last = 1000000;
    float z_last = 1000000;

    int x_trend = -1;
    int y_trend = -1;
    int z_trend = -1;

    long long x_last_time=0;
    long long y_last_time=0;
    long long z_last_time=0;

    int onCheatHandler(std::list<SensorValue> *x_values, std::list<SensorValue> *y_values,
                       std::list<SensorValue> *z_values);

    bool equals_interval(int sum_interval, std::list<int> *time_intervals) const;

    bool *identifyLine(std::list<SensorValue> *values) const;

    void filter_x(float value, long long time_stamp);

    void filter_y(float value, long long time_stamp);

    void filter_z(float value, long long time_stamp);

    static std::list<SensorValue> *getLastSensorValue(std::list<SensorValue> *all_values);


public:
    //摇步器作弊指数

    CheatSensorHandler();

    void addSensorValue(float x_angle, float y_angle, float z_angle, long long time_stamp);

    void clean();

    int computerWalkerCheat();

};

#endif //SCAVENGER_CHEATSENSORHANDLER_H

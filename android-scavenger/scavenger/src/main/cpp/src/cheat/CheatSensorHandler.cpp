//
// Created by liruopeng on 2022/12/9.
//

#include <android/log.h>
#include "CheatSensorHandler.h"

int CheatSensorHandler::onCheatHandler(std::list <SensorValue> *x_value_list,
                                       std::list <SensorValue> *y_value_list,
                                       std::list <SensorValue> *z_value_list) {
    ///判断波峰波谷时间间隔 10
    ///判断是否绕着一个轴转 20
    ///相邻的波峰波谷之间的平均数，与总平均数是否重合。40
    ///波峰波谷总是在一个数值上80

//    __android_log_print(ANDROID_LOG_INFO, "liruopeng", "x length：%d", x_value_list->size());
//    __android_log_print(ANDROID_LOG_INFO, "liruopeng", "y length：%d", y_value_list->size());
//    __android_log_print(ANDROID_LOG_INFO, "liruopeng", "z length：%d", z_value_list->size());

    if (x_value_list->size() < 60 || y_value_list->size() < 60 || z_value_list->size() < 60) {
        x_value_list->clear();
        y_value_list->clear();
        z_value_list->clear();
        return 0;
    }

    int cheat_index = 0;
    int sumIntervalX = 0;

    auto *xTimeInterval = new std::list<int>;
    auto *yTimeInterval = new std::list<int>;
    auto *zTimeInterval = new std::list<int>;
    for (auto iter = x_value_list->begin();
         iter != x_value_list->end();) {
        int timeInterval = abs((int) (iter->getMillisecond() - (iter++->getMillisecond())));
        sumIntervalX += timeInterval;
        xTimeInterval->push_back(timeInterval);
    }
    int sumIntervalY = 0;
    for (auto iter = y_value_list->begin();
         iter != y_value_list->end();) {
        int timeInterval = abs((int) (iter->getMillisecond() - (iter++->getMillisecond())));
        sumIntervalY += timeInterval;
        yTimeInterval->push_back(timeInterval);
    }
    int sumIntervalZ = 0;
    for (auto iter = z_value_list->begin();
         iter != z_value_list->end();) {
        int timeInterval = abs((int) (iter->getMillisecond() - (iter++->getMillisecond())));
        sumIntervalZ += timeInterval;
        zTimeInterval->push_back(timeInterval);
    }

    //时间间隔一样
    //x时间间隔一样
    if (equals_interval(sumIntervalX, xTimeInterval) ||
        equals_interval(sumIntervalY, yTimeInterval) ||
        equals_interval(sumIntervalZ, zTimeInterval)) {
        cheat_index += 10;
    }

    auto xIdentifyLine = identifyLine(x_value_list);
    auto yIdentifyLine = identifyLine(y_value_list);
    auto zIdentifyLine = identifyLine(z_value_list);
    int lineOfAxis = 0;
    if (xIdentifyLine[0]) {
        lineOfAxis++;
    }
    if (yIdentifyLine[0]) {
        lineOfAxis++;
    }
    if (zIdentifyLine[0]) {
        lineOfAxis++;
    }
    if (lineOfAxis == 1 || lineOfAxis == 2) {
        cheat_index += 20;
    }

    if (xIdentifyLine[3] || yIdentifyLine[3] || zIdentifyLine[3]) {
        cheat_index += 40;
    }
    if ((!xIdentifyLine[0] && (xIdentifyLine[1] && xIdentifyLine[2]))
        || (!yIdentifyLine[0] && (yIdentifyLine[1] && yIdentifyLine[2]))
        || (!zIdentifyLine[0] && (zIdentifyLine[1] && zIdentifyLine[2]))) {
        cheat_index += 80;
    }
    delete[] xIdentifyLine;
    delete[] yIdentifyLine;
    delete[] zIdentifyLine;
    xTimeInterval->clear();
    yTimeInterval->clear();
    zTimeInterval->clear();
    x_value_list->clear();
    y_value_list->clear();
    z_value_list->clear();
    return cheat_index;
}

bool *CheatSensorHandler::identifyLine(std::list <SensorValue> *values) const {
    if (values->size() <= 10) {
        return new bool[4]{true, false, false, false};
    }
    //是否是直线
    bool isLine;
    //波峰是否是一条直线
    bool isCrestLine;
    //波谷是否是一条直线
    bool isTroughLine;
    //是否点平均分布
    bool isPointAvg;

    float totalValue = 0;
    float totalCrestValue = 0;
    int totalCrestSize = 0;
    float totalTroughValue = 0;
    int totalTroughSize = 0;

    auto *pointAvgList = new std::list<float>;
    SensorValue *lastSensorValue = nullptr;

    for (auto &value: *values) {
        if (value.getWaveType() == 1) {
            totalCrestValue += value.getValue();
            totalCrestSize++;
        } else if (value.getWaveType() == 2) {
            totalTroughValue += value.getValue();
            totalTroughSize++;
        }
        totalValue += value.getValue();
        if (lastSensorValue != nullptr) {
            pointAvgList->push_back((value.getValue() + lastSensorValue->getValue()) / 2);
        }
        lastSensorValue = &value;
    }

    float avgXValue = totalValue / ((float) values->size());
    float avgCrestXValue = totalCrestValue / (float) totalCrestSize;
    float avgTroughXValue = totalTroughValue / (float) totalTroughSize;

    int lineXCount = 0;
    int lineCrestXCount = 0;
    int lineTroughXCount = 0;

    for (auto &value: *values) {
        if (abs(value.getValue() - avgXValue) <= line_offset) {
            lineXCount++;
        }
        if (value.getWaveType() == 1) {
            if (abs(value.getValue() - avgCrestXValue) <=
                abs(avgCrestXValue - avgTroughXValue) * 0.1) {
                lineCrestXCount++;
            }
        } else if (value.getWaveType() == 2) {
            if (abs(value.getValue() - avgTroughXValue) <=
                abs(avgCrestXValue - avgTroughXValue) * 0.1) {
                lineTroughXCount++;
            }
        }
    }
    isLine = ((float) (lineXCount) / ((float) values->size())) > rate_of_accuracy;
    isCrestLine = ((float) (lineCrestXCount) / (float) totalCrestSize) > rate_of_accuracy;
    isTroughLine = ((float) (lineTroughXCount) / (float) totalTroughSize) > rate_of_accuracy;

    int lineAvgCount = 0;
    for (float &iter: *pointAvgList) {
        if (abs(iter - avgXValue) < abs(avgCrestXValue - avgTroughXValue) * 0.1) {
            lineAvgCount++;
        }
    }
    isPointAvg = !pointAvgList->empty() &&
                 ((float) (lineAvgCount) / ((float) pointAvgList->size())) > rate_of_accuracy &&
                 !isLine;

    pointAvgList->clear();
    return new bool[]{isLine, isCrestLine, isTroughLine, isPointAvg};
}

bool CheatSensorHandler::equals_interval(int sum_interval, std::list<int> *time_intervals) const {
    if (time_intervals->empty()) {
        return false;
    }
    int avgIntervalX = sum_interval / time_intervals->size();
    int checkNum = 0;
    for (int &time_interval: *time_intervals) {
        if (abs(time_interval - avgIntervalX) < time_offset) {
            checkNum++;
        }
    }
    return ((float(checkNum)) / (float(time_intervals->size()))) > rate_of_accuracy;
}

/**
 * X轴数据提取
 * @param value
 * @param time_stamp
 */
void CheatSensorHandler::filter_x(float value, long long time_stamp) {
    if (x_last == 1000000 || x_trend == -1) {
        x_last = value;
        x_trend = 0;
        return;
    }
    if (x_trend == 0) {
        x_trend = value < x_last ? 1 : 2;
        x_last = value;
        return;
    }

    if ((time_stamp - x_last_time) > 20) {
        if ((value < x_last && x_trend == 2) || (value > x_last && x_trend == 1)) {
            x_last_time = time_stamp;
            x_last = value;
            x_trend = x_trend == 2 ? 1 : 2;
            auto *sensorValue = new SensorValue(value, time_stamp, x_trend);
            x_values->push_back(*sensorValue);
            delete (sensorValue);
            return;
        }
    }
    x_last = value;
}

/**
 * Y轴数据提取
 * @param value
 * @param time_stamp
 */
void CheatSensorHandler::filter_y(float value, long long time_stamp) {
    if (y_last == 1000000 || y_trend == -1) {
        y_last = value;
        y_trend = 0;
        return;
    }
    if (y_trend == 0) {
        y_trend = value < y_last ? 1 : 2;
        y_last = value;
        return;
    }
    if ((time_stamp - y_last_time) > 20) {
        if ((value < y_last && y_trend == 2) || (value > y_last && y_trend == 1)) {
            y_last_time = time_stamp;
            y_last = value;
            y_trend = y_trend == 2 ? 1 : 2;
            auto *sensorValue = new SensorValue(value, time_stamp, y_trend);
            y_values->push_back(*sensorValue);
            delete (sensorValue);
            return;
        }
    }
    y_last = value;
}

/**
 * Z轴数据提取
 * @param value
 * @param time_stamp
 */
void CheatSensorHandler::filter_z(float value, long long time_stamp) {
    if (z_last == 1000000 || z_trend == -1) {
        z_last = value;
        z_trend = 0;
        return;
    }
    if (z_trend == 0) {
        z_trend = value < z_last ? 1 : 2;
        z_last = value;
        return;
    }
    if ((time_stamp - z_last_time) > 20) {
        if ((value < z_last && z_trend == 2) || (value > z_last && z_trend == 1)) {
//        __android_log_print(ANDROID_LOG_INFO, "liruopeng", "--------------------------------");
//        __android_log_print(ANDROID_LOG_INFO, "liruopeng", "--z：%f", value);
//        __android_log_print(ANDROID_LOG_INFO, "liruopeng", "--time：%lld", time_stamp);
//        __android_log_print(ANDROID_LOG_INFO, "liruopeng", "--z_last：%f", z_last);
//        __android_log_print(ANDROID_LOG_INFO, "liruopeng", "--z_trend：%d", z_trend);
//        __android_log_print(ANDROID_LOG_INFO, "liruopeng", "--------------------------------");
            z_last_time = time_stamp;
            z_last = value;
            z_trend = z_trend == 2 ? 1 : 2;
            auto *sensorValue = new SensorValue(value, time_stamp, z_trend);
            z_values->push_back(*sensorValue);
            delete (sensorValue);
            return;
        }
    }

    z_last = value;
}

/**
 * 获取最后一分钟内的波峰波点
 * @return
 */
std::list <SensorValue> *
CheatSensorHandler::getLastSensorValue(std::list <SensorValue> *all_values) {
    auto *valueList = new std::list<SensorValue>;
    if (all_values->empty()) {
        return valueList;
    }
    auto *lastValue = &all_values->back();
    for (auto iter = all_values->rbegin();
         iter != all_values->rend(); ++iter) {
        if (lastValue->getMillisecond() - iter->getMillisecond() < 60 * 1000) {
            valueList->push_front(*iter);
        }
    }
    return valueList;
}


CheatSensorHandler::CheatSensorHandler() = default;


void CheatSensorHandler::addSensorValue(float x_angle, float y_angle, float z_angle,
                                        long long time_stamp) {


    filter_x(x_angle, time_stamp);
    filter_y(y_angle, time_stamp);
    filter_z(z_angle, time_stamp);
//    walker_cheat_index = onCheatHandler(getLastSensorValue(x_values), getLastSensorValue(y_values),
//                                        getLastSensorValue(z_values));
//    __android_log_print(ANDROID_LOG_INFO, "liruopeng", "walker_cheat_index：%d", walker_cheat_index);

}


void CheatSensorHandler::clean() {
    x_values->clear();
    y_values->clear();
    z_values->clear();
    x_last = 1000000;
    y_last = 1000000;
    z_last = 1000000;
    x_trend = -1;
    y_trend = -1;
    z_trend = -1;
    x_last_time = 0;
    y_last_time = 0;
    z_last_time = 0;
}

int CheatSensorHandler::computerWalkerCheat() {
    int walker_cheat_index = onCheatHandler(getLastSensorValue(x_values),
                                            getLastSensorValue(y_values),
                                            getLastSensorValue(z_values));
    __android_log_print(ANDROID_LOG_INFO, "liruopeng", "walker_cheat_index：%d", walker_cheat_index);
    return walker_cheat_index;
}






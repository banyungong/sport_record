//
// Created by liruopeng on 2022/8/24.
//

#include <stdio.h>
#include "KalmanTool.h"
#include "math.h"

void KalmanTool::initial() {
    pdelt_x = 0.001;
    pdelt_y = 0.001;
    mdelt_x = 5.698402909980532E-4;
    mdelt_y = 5.698402909980532E-4;
}

double *
KalmanTool::kalmanFilter(double oldValue_x, double value_x, double oldValue_y, double value_y) {
    lastLocation_x = oldValue_x;
    currentLocation_x = value_x;

    gauss_x = sqrt(pdelt_x * pdelt_x + mdelt_x * mdelt_x) + m_Q;     //计算高斯噪音偏差
    kalmanGain_x =
            sqrt((gauss_x * gauss_x) / (gauss_x * gauss_x + pdelt_x * pdelt_x)) + m_R; //计算卡尔曼增益
    estimate_x = kalmanGain_x * (currentLocation_x - lastLocation_x) + lastLocation_x;    //修正定位点
    mdelt_x = sqrt((1 - kalmanGain_x) * gauss_x * gauss_x);      //修正模型偏差

    lastLocation_y = oldValue_y;
    currentLocation_y = value_y;
    gauss_y = sqrt(pdelt_y * pdelt_y + mdelt_y * mdelt_y) + m_Q;     //计算高斯噪音偏差
    kalmanGain_y =
            sqrt((gauss_y * gauss_y) / (gauss_y * gauss_y + pdelt_y * pdelt_y)) + m_R; //计算卡尔曼增益
    estimate_y = kalmanGain_y * (currentLocation_y - lastLocation_y) + lastLocation_y;    //修正定位点
    mdelt_y = sqrt((1 - kalmanGain_y) * gauss_y * gauss_y);
    auto *result = new double[2];
    result[0] = estimate_y;
    result[1] = estimate_x;
    return result;
}

CPoint *KalmanTool::kalmanFilterPoint(CPoint *lastLoc, CPoint *curLoc, int intensity) {
    if (pdelt_x == 0 || pdelt_y == 0) {
        initial();
    }
    if (lastLoc == nullptr || curLoc == nullptr) {
        return lastLoc;
    }
    if (intensity < 1) {
        intensity = 1;
    } else if (intensity > 5) {
        intensity = 5;
    }

    double oldValue_x = lastLoc->longitude / 1000000.0;
    double value_x = curLoc->longitude / 1000000.0;
    double oldValue_y = lastLoc->latitude / 1000000.0;
    double value_y = curLoc->latitude / 1000000.0;

    for (int j = 0; j < intensity; j++) {
        double *result = kalmanFilter(oldValue_x, value_x, oldValue_y,
                                      value_y);
        value_y = result[0];
        value_x = result[1];
        delete result;
    }
    curLoc->latitude = (static_cast<int>(value_y * 1000000));
    curLoc->longitude = (static_cast<int>(value_x * 1000000));
    return curLoc;
}

void KalmanTool::clean() {
    lastLocation_x = 0.0;
    currentLocation_x = 0.0;//这次位置
    lastLocation_y = 0.0; //上次位置
    currentLocation_y = 0.0;//这次位置
    estimate_x = 0.0; //修正后数据
    estimate_y = 0.0; //修正后数据
    pdelt_x = 0.0; //自预估偏差
    pdelt_y = 0.0; //自预估偏差
    mdelt_x = 0.0; //上次模型偏差
    mdelt_y = 0.0; //上次模型偏差
    gauss_x = 0.0; //高斯噪音偏差
    gauss_y = 0.0; //高斯噪音偏差
    kalmanGain_x = 0.0; //卡尔曼增益
    kalmanGain_y = 0.0; //卡尔曼增益
    m_R = 0;
    m_Q = 0;
}

CPoint *KalmanTool::kalmanFilterPoint(CPoint *lastLoc, CPoint *curLoc) {
    return kalmanFilterPoint(lastLoc, curLoc, 3);
}

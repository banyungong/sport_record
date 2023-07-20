//
// Created by liruopeng on 2022/8/24.
//

#ifndef JOYRUN_JNI_KALMANTOOL_H
#define JOYRUN_JNI_KALMANTOOL_H

#include "../model/cpoint.h"

class KalmanTool {
private:
    double lastLocation_x;
    double currentLocation_x;//这次位置
    double lastLocation_y; //上次位置
    double currentLocation_y;//这次位置
    double estimate_x; //修正后数据
    double estimate_y; //修正后数据
    double pdelt_x; //自预估偏差
    double pdelt_y; //自预估偏差
    double mdelt_x; //上次模型偏差
    double mdelt_y; //上次模型偏差
    double gauss_x; //高斯噪音偏差
    double gauss_y; //高斯噪音偏差
    double kalmanGain_x; //卡尔曼增益
    double kalmanGain_y; //卡尔曼增益
    double m_R = 0;
    double m_Q = 0;

    /**
     * 初始化模型
     */
    void initial();

    /**
     * 滤波算法开始
     */
    double *kalmanFilter(double oldValue_x, double value_x, double oldValue_y, double value_y);

public:
    /**
     * 单点滤波
     * @param lastLoc   上次定位点坐标
     * @param curLoc    本次定位点坐标
     * @param intensity 滤波强度（1—5）
     * @return 滤波后本次定位点坐标值
     */
    CPoint *kalmanFilterPoint(CPoint *lastLoc, CPoint *curLoc, int intensity);

    void clean();

    CPoint *kalmanFilterPoint(CPoint *lastLoc, CPoint *curLoc);
};


#endif //JOYRUN_JNI_KALMANTOOL_H

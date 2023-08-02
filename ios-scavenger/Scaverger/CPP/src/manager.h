//
// Created by liruopeng on 2023/6/25.
//

#ifndef SCAVENGER_MANAGER_H
#define SCAVENGER_MANAGER_H


#include "./model/cpoint.h"
#include "./model/result_point.h"
#include "handler/kalman_handler.h"
#include "handler/shake_cheat_handler.h"
#include "handler/default_cheat_handler.h"
#include "handler/mileage_handler.h"
#include "handler/calorie_handler.h"
#include "handler/climb_handler.h"
#include "handler/pace_handler.h"
#include "storage/storage_handler.h"

class Manager {
private:
    std::list<CPoint> *point_list = new std::list<CPoint>;
    //卡尔曼优化
    KalmanHandler *kalmanHandler;
    //简单防作弊
    DefaultCheatHandler *defaultCheatHandler;
    //防止摇步器作弊
    ShakeCheatHandler *shakeCheatHandler;
    //计算点里程
    MileageHandler *mileageHandler;
    //计算卡路里
    CalorieHandler *calorieHandler;
    //计算爬升
    ClimbHandler *climbHandler;
    //计算配速
    PaceHandler *paceHandler;

public:
    Manager();

    ~Manager();

    ResultPoint *addPoint(CPoint *cPoint);

    /**
     * 初始化
     * @param dir 文件存储目录
     * @param tag 文件标识
     * @param kalman_intensity 卡尔曼滤波强度1-5
     * @param shake_intensity 默认作弊强度1-5
     */
    void init(int kalman_intensity, int shake_intensity);

    void clean();
};


#endif //SCAVENGER_MANAGER_H

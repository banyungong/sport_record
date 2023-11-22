//
// Created by liruopeng on 2023/6/25.
//

#include "manager.h"

ResultPoint *Manager::addPoint(CPoint *cPoint) {
    auto *resultPoint = new ResultPoint();
    kalmanHandler->onHandler(point_list, cPoint, resultPoint);
    shakeCheatHandler->onHandler(point_list, cPoint, resultPoint);
    if (resultPoint->type == CPoint_TYPE::TYPE_NORMAL) {
        defaultCheatHandler->onHandler(point_list, cPoint, resultPoint);
    }
    mileageHandler->onHandler(point_list, cPoint, resultPoint);
    if (resultPoint->type == 0) {
        paceHandler->onHandler(point_list, cPoint, resultPoint);
        climbHandler->onHandler(point_list, cPoint, resultPoint);
        calorieHandler->onHandler(point_list, cPoint, resultPoint);
    }
    resultPoint->longitude = cPoint->longitude;
    resultPoint->latitude = cPoint->latitude;
    if (point_list->empty()) {
        resultPoint->step = 0;
    } else {
        resultPoint->step = cPoint->step - point_list->back().step;
    }
    point_list->push_back(*cPoint);
    //最多保留120个点
    if (point_list->size() > 120) {
        int i = 0;
        auto *temp_point_list = new std::list<CPoint>();
        for (auto it = point_list->rbegin(); it != point_list->rend(); it++) {
            if (i >= 60) {
                break;
            }
            temp_point_list->push_front(*it);
            i++;
        }
        point_list->clear();
        point_list = temp_point_list;
    }
    return resultPoint;
}

Manager::Manager() {
    kalmanHandler = new KalmanHandler();
    shakeCheatHandler = new ShakeCheatHandler();
    defaultCheatHandler = new DefaultCheatHandler();
    mileageHandler = new MileageHandler();
    paceHandler = new PaceHandler();
    calorieHandler = new CalorieHandler();
    climbHandler = new ClimbHandler();
}

Manager::~Manager() {
    delete kalmanHandler;
    delete shakeCheatHandler;
    delete defaultCheatHandler;
    delete mileageHandler;
    delete paceHandler;
    delete calorieHandler;
    delete climbHandler;
}

void Manager::clean() {
    point_list->clear();
    kalmanHandler->clean();
    shakeCheatHandler->clean();
    defaultCheatHandler->clean();
    mileageHandler->clean();
    paceHandler->clean();
    calorieHandler->clean();
    climbHandler->clean();
}

//10 20 40 80 150
void Manager::init(int kalman_intensity,
                   int shake_intensity) {
    kalmanHandler->intensity = kalman_intensity;
    switch (shake_intensity) {
        case 1:
            shake_intensity = 30;
            break;
        case 2:
            shake_intensity = 40;
            break;
        case 3:
            shake_intensity = 60;
            break;
        case 4:
            shake_intensity = 80;
            break;
        case 5:
            shake_intensity = 150;
            break;
        default:
            shake_intensity = 60;
            break;
    }
    shakeCheatHandler->intensity = shake_intensity;
}





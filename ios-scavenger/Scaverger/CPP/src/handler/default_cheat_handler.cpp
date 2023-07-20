//
// Created by liruopeng on 2023/6/28.
//

#include "default_cheat_handler.h"
#include "../utils/CoordinateUtils.h"
//#include <android/log.h>


bool DefaultCheatHandler::onHandler(std::list<CPoint> *point_list, CPoint *point) {
    if (point_list->size() < 5) {
        return false;
    }
    //获取point_list中最近10秒内的点
    std::list<CPoint> *recent_point_list = new std::list<CPoint>();
    for (auto it = point_list->rbegin(); it != point_list->rend(); it++) {
        if (point->timestamp - it->timestamp < 10&&recent_point_list->size()<5) {
            recent_point_list->push_back(*it);
        } else {
            //如果已经超过15秒，就不再继续往前找了
            break;
        }
    }
    //如果recent_point_list为空，退出
    if (recent_point_list->size() == 0) {
        delete recent_point_list;
        return false;
    }
    //计算时间差
    range_second = point->timestamp - recent_point_list->back().timestamp;
    //计算步数差
    range_step = point->step - recent_point_list->back().step;
    //循环recent_point_list，每格2秒，计算距离差
    CPoint *_temp_point = point;
    range_distance = 0;
    for (auto it = recent_point_list->begin(); it != recent_point_list->end(); it++) {
        if (_temp_point->timestamp - it->timestamp >= 2) {
            int distance = CoordinateUtils::getDistance(&*it, _temp_point);
            range_distance += distance;
            _temp_point = &*it;
        }
    }

    CPoint_TYPE speedType = checkSpeed();
    if (speedType != TYPE_NORMAL) {
        point->type = speedType;
        return false;
    }
    CPoint_TYPE stepType = checkStep();
    if (stepType != TYPE_NORMAL) {
        point->type = stepType;
        return false;
    }
    recent_point_list->clear();
    return CHandler::onHandler(point_list, point);
}

CPoint_TYPE DefaultCheatHandler::checkSpeed() {
    //如果时间等于0，退出
    if (range_second <= 0) {
        return TYPE_NORMAL;
    }
    //如果距离等于0，退出
    if (range_distance <= 0) {
        return TYPE_NORMAL;
    }
    //计算速度
    double pace = 100000.0 / range_distance * range_second;
//    __android_log_print(ANDROID_LOG_INFO, "liruopeng", "range_distance：%d",range_distance);
//    __android_log_print(ANDROID_LOG_INFO, "liruopeng", "range_second：%d",range_second);

    if (pace < 100) {
        return TYPE_PACE_ERROR1;
    }

    return TYPE_NORMAL;
}

CPoint_TYPE DefaultCheatHandler::checkStep() {
    //如果时间等于0，退出
    if (range_second <= 0) {
        return TYPE_NORMAL;
    }
//    __android_log_print(ANDROID_LOG_INFO, "liruopeng", "range_step：%d",range_step);

    //计算步频
    int cadence = 60 * (range_step / range_second);
    if (cadence > 240) {
        return TYPE_STEP_CADENCE;
    }
    //计算步幅
    // 步幅限制
    if (range_step <= 0) {
        if (range_second >= 5 && range_distance > 100 * 10) {
            return TYPE_STEP_ERROR2;
        } else {
            return TYPE_STEP_ERROR3;
        }
    } else {
        int step_stride = range_distance / range_step;
        if (step_stride > 300) {
            return TYPE_STEP_ERROR1;
        }
    }
    return TYPE_NORMAL;
}

void DefaultCheatHandler::clean() {
    range_distance = 0;
    range_step = 0;
    range_second = 0;
}

//
// Created by liruopeng on 2023/7/21.
//

#include "mileage_handler.h"
#include "../utils/CoordinateUtils.h"

bool
MileageHandler::onHandler(std::list<CPoint> *point_list, CPoint *inPoint, ResultPoint *outPoint) {
    //计算当前点距离
    //如果point_list为空，meter等于0
    if (point_list->empty()) {
        inPoint->meter = 0;
        outPoint->meter = 0;
        return false;
    }

    //获取前5s的点
    auto *list = new std::list<CPoint>();
    for (auto it = point_list->rbegin(); it != point_list->rend(); it++) {
        if (it->timestamp > inPoint->timestamp - 5) {
            list->push_back(*it);
        }
    }
    list->push_front(*inPoint);
    //如果点数小于2，meter等于point_list的最后一个点的meter
    if (list->size() < 2) {
        inPoint->meter = point_list->back().meter;
        outPoint->meter = inPoint->meter;
        list->clear();
        return false;
    }
    //如果点数大于等于2，先抽稀，再计算距离
    //抽稀
    std::list<CPoint> *temp_list = pathSmoothTool->reducerVerticalThreshold(list, 0.5);
    //计算temp_list第一个点和第二个点的距离
    if (temp_list->size() > 1) {
        temp_list->pop_front();
        CPoint *temp_point = &temp_list->front();
        int distance = CoordinateUtils::getDistance(inPoint, temp_point);
        inPoint->meter = (temp_point->meter + distance);
    } else {
        inPoint->meter = temp_list->back().meter;
    }
    outPoint->meter = inPoint->meter;
    list->clear();
    temp_list->clear();
    return false;
}

void MileageHandler::clean() {
    CHandler::clean();
}

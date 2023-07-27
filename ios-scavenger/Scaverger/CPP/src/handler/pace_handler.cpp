//
// Created by liruopeng on 2023/7/24.
//

#include "pace_handler.h"

bool PaceHandler::onHandler(std::list<CPoint> *point_list, CPoint *inPoint, ResultPoint *outPoint) {
    //计算当前点配速
    //如果point_list为空，pace等于0
    if (point_list->empty()) {
        outPoint->pace = 9999;
        return false;
    }
    int meter_temp;
    int second_temp;
    for (auto it = point_list->rbegin(); it != point_list->rend(); it++) {
        if (inPoint->timestamp - it->timestamp > 15) {
            break;
        }
        meter_temp = inPoint->meter - it->meter;
        second_temp = inPoint->timestamp - it->timestamp;
        if (meter_temp > 0 && second_temp > 0) {
            outPoint->pace = 100000 / meter_temp * second_temp;
            break;
        }
    }
    if (meter_temp == 0 || second_temp == 0) {
        outPoint->pace = 9999;
    }
    return false;
}

void PaceHandler::clean() {
    CHandler::clean();
}

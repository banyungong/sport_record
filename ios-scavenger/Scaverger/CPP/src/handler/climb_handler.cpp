//
// Created by liruopeng on 2023/7/24.
//

#include "climb_handler.h"

bool
ClimbHandler::onHandler(std::list<CPoint> *point_list, CPoint *inPoint, ResultPoint *outPoint) {
//    if (point_list->empty()) {
//        outPoint->climb = 0;
//        return false;
//    }
//    CPoint last_point = point_list->back();
//    outPoint->climb = cPoint->climb + (cPoint->gpsAltitude - last_point.gpsAltitude);
//    return CHandler::onHandler(point_list, cPoint);
    return false;
}

void ClimbHandler::clean() {
    CHandler::clean();
}

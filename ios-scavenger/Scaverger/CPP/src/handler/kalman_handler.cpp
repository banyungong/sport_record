//
// Created by liruopeng on 2023/6/25.
//
#include "kalman_handler.h"
//#include <android/log.h>

KalmanHandler::KalmanHandler() {
    kalmanTool = new KalmanTool();
}

KalmanHandler::~KalmanHandler() {
    kalmanTool->clean();
    kalmanTool = nullptr;
}

bool KalmanHandler::onHandler(std::list<CPoint> *point_list, CPoint *inPoint, ResultPoint *outPoint) {
    if (point_list->empty()) {
        return false;
    }
    if (inPoint == nullptr) {
        return false;
    }
    CPoint *last = &point_list->back();
    if ((inPoint->timestamp - last->timestamp) <= 5) {
        kalmanTool->kalmanFilterPoint(last, inPoint,intensity);
    }
    return false;
}

void KalmanHandler::clean() {
    kalmanTool->clean();
}

//
// Created by liruopeng on 2023/6/25.
//

#include "manager.h"
#include <android/log.h>

CPoint *Manager::addPoint(CPoint *cPoint) {
    handler = kalmanHandler;
    handler->onHandler(point_list, cPoint);
    handler = shakeCheatHandler;
    handler->onHandler(point_list, cPoint);
    if (cPoint->type == CPoint_TYPE::TYPE_NORMAL) {
        handler = defaultCheatHandler;
        handler->onHandler(point_list, cPoint);
    }
    point_list->push_back(*cPoint);

    //最多保留120个点
    if (point_list->size() > 120) {
        int i = 0;
        std::list<CPoint> *temp_point_list = new std::list<CPoint>();
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
    __android_log_print(ANDROID_LOG_INFO, "liruopeng", "point_list length：%d", point_list->size());
    return cPoint;
}

Manager::Manager() {
    kalmanHandler = new KalmanHandler();
    shakeCheatHandler = new ShakeCheatHandler();
    defaultCheatHandler = new DefaultCheatHandler();
}

Manager::~Manager() {
    delete kalmanHandler;
    delete shakeCheatHandler;
    delete defaultCheatHandler;
    delete handler;
}

void Manager::clean() {
    point_list->clear();
    kalmanHandler->clean();
    shakeCheatHandler->clean();
    defaultCheatHandler->clean();
}

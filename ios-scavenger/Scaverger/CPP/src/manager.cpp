//
// Created by liruopeng on 2023/6/25.
//

#include "manager.h"

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

//    //最多保留120个点
//    if(point_list->size()>60){
//        point_list->pop_front();
//    }
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
//    delete handler;
    delete point_list;
}

void Manager::clean() {
//    point_list->clear();
    kalmanHandler->clean();
    shakeCheatHandler->clean();
    defaultCheatHandler->clean();
}

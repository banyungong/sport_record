//
// Created by liruopeng on 2023/6/25.
//

#ifndef SCAVENGER_MANAGER_H
#define SCAVENGER_MANAGER_H


#include "./model/cpoint.h"
#include "handler/kalman_handler.h"
#include "handler/shake_cheat_handler.h"
#include "handler/default_cheat_handler.h"

class Manager {

private:
    std::list<CPoint> *point_list = new std::list<CPoint>;
    KalmanHandler *kalmanHandler;
    DefaultCheatHandler *defaultCheatHandler;
    ShakeCheatHandler *shakeCheatHandler;
    CHandler *handler;
public:
    Manager();

    ~Manager();

    CPoint *addPoint(CPoint *cPoint);

    void clean();
    
};


#endif //SCAVENGER_MANAGER_H

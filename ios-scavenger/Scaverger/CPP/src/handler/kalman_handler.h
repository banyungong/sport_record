//
// Created by liruopeng on 2023/6/25.
//

#ifndef SCAVENGER_KALMAN_HANDLER_H
#define SCAVENGER_KALMAN_HANDLER_H

#include "handler.h"
#include "../kalman/KalmanTool.h"

class KalmanHandler : public CHandler {
private:
    KalmanTool *kalmanTool;
public:
    int intensity = 3;

    KalmanHandler();

    ~KalmanHandler();

    bool onHandler(std::list<CPoint> *point_list, CPoint *inPoint, ResultPoint *outPoint) override;

    void clean() override;
};

#endif //SCAVENGER_KALMAN_HANDLER_H

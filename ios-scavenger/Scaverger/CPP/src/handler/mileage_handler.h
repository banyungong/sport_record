//
// Created by liruopeng on 2023/7/21.
// 计算里程
//

#ifndef ANDROID_SCAVENGER_MILEAGE_HANDLER_H
#define ANDROID_SCAVENGER_MILEAGE_HANDLER_H

#include "handler.h"
#include "list"
#include "../utils/PathSmoothTool.h"
#include "../model/cpoint.h"

class MileageHandler : public CHandler {
private:
    PathSmoothTool *pathSmoothTool = new PathSmoothTool();
public:
    bool onHandler(std::list<CPoint> *point_list, CPoint *inPoint, ResultPoint *outPoint) override;

    void clean() override;

};


#endif //ANDROID_SCAVENGER_MILEAGE_HANDLER_H

//
// Created by liruopeng on 2023/7/24.
//

#ifndef ANDROID_SCAVENGER_CALORIE_HANDLER_H
#define ANDROID_SCAVENGER_CALORIE_HANDLER_H

#include "handler.h"
#include "list"

class CalorieHandler : public CHandler{
public:
    bool onHandler(std::list <CPoint> *point_list, CPoint *inPoint, ResultPoint *outPoint) override;

    void clean() override;
};


#endif //ANDROID_SCAVENGER_CALORIE_HANDLER_H

//
// Created by liruopeng on 2023/7/24.
//

#include "calorie_handler.h"

bool
CalorieHandler::onHandler(std::list<CPoint> *point_list, CPoint *inPoint, ResultPoint *outPoint) {
    if (point_list->empty()) {
        outPoint->calorie = 0;
        return false;
    }

    return false;
}

void CalorieHandler::clean() {
    CHandler::clean();
}

//
// Created by liruopeng on 2023/6/28.
//

#include "CoordinateUtils.h"

double CoordinateUtils::getDistance(double lat1, double lng1, double lat2, double lng2) {
    double x, y, distance;
    x = (lng2 - lng1) * M_PI * EARTH_R * cos(((lat1 + lat2) / 2) * M_PI / 180) / 180;
    y = (lat2 - lat1) * M_PI * EARTH_R / 180;
    distance = hypot(x, y);
    return distance;
}

int CoordinateUtils::getDistance(CPoint *arg1, CPoint *arg2) {
    if (arg1 == NULL || arg2 == NULL) {
        return 0;
    }
    double meter = CoordinateUtils::getDistance(arg1->latitude / 1000000.0,
                                                arg1->longitude / 1000000.0,
                                                arg2->latitude / 1000000.0,
                                                arg2->longitude / 1000000.0);
    int cmeter = meter * 100;
    return cmeter;
}

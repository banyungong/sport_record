//
// Created by liruopeng on 2023/6/28.
//

#ifndef SCAVENGER_COORDINATEUTILS_H
#define SCAVENGER_COORDINATEUTILS_H

#include "math.h"
#include "../model/cpoint.h"

#define EARTH_R  6371229 // 地球的半径

class CoordinateUtils {
public:
    static double getDistance(double lat1, double lng1, double lat2, double lng2);

    ///返回的是厘米
    static int getDistance(CPoint *arg1, CPoint *arg2);
};


#endif //SCAVENGER_COORDINATEUTILS_H

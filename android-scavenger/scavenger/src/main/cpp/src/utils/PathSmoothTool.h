#ifndef SCAVENGER_PATHSMOOTHTOOL_H
#define SCAVENGER_PATHSMOOTHTOOL_H

#include "../model/cpoint.h"
#include "list"

class PathSmoothTool {
private:

    std::list <CPoint> *reduceNoisePoint(std::list <CPoint> *inPoints, float threshold);

    double calculateDistanceFromPoint(CPoint *p, CPoint *lineBegin, CPoint *lineEnd);

public:
    std::list <CPoint> *reducerVerticalThreshold(std::list <CPoint> *inPoints, float threshold);

};

#endif //SCAVENGER_PATHSMOOTHTOOL_H
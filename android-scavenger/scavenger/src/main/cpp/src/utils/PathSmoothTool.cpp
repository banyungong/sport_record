#ifndef SCAVENGER_PATHSMOOTHTOOL_CPP
#define SCAVENGER_PATHSMOOTHTOOL_CPP

#include "PathSmoothTool.h"
#include <CoordinateUtils.h>

std::list <CPoint> *
PathSmoothTool::reduceNoisePoint(std::list <CPoint> *inPoints, float threshold) {

    if (inPoints == nullptr || inPoints->size() < 3) {
        return inPoints;
    }
    std::list <CPoint> *outPoints = new std::list<CPoint>();
    std::list<CPoint>::iterator it = inPoints->begin();
    CPoint *lastPoint = &(*it);
    outPoints->push_back(*lastPoint);
    it++;
    for (; it != inPoints->end(); it++) {
        CPoint *currentPoint = &(*it);
        double distance = calculateDistanceFromPoint(currentPoint, lastPoint,
                                                     &(*outPoints->rbegin()));
        if (distance > threshold) {
            outPoints->push_back(*currentPoint);
            lastPoint = currentPoint;
        }
    }
    return outPoints;
}

double PathSmoothTool::calculateDistanceFromPoint(CPoint *p, CPoint *lineBegin, CPoint *lineEnd) {
    double a = p->longitude - lineBegin->longitude;
    double b = p->latitude - lineBegin->latitude;
    double c = lineEnd->longitude - lineBegin->longitude;
    double d = lineEnd->latitude - lineBegin->latitude;

    double dot = a * c + b * d;
    double len_sq = c * c + d * d;
    double param = dot / len_sq;

    double xx, yy;

    if (param < 0 ||
        (lineBegin->longitude == lineEnd->longitude && lineBegin->latitude == lineEnd->latitude)) {
        xx = lineBegin->longitude;
        yy = lineBegin->latitude;
    } else if (param > 1) {
        xx = lineEnd->longitude;
        yy = lineEnd->latitude;
    } else {
        xx = lineBegin->longitude + param * c;
        yy = lineBegin->latitude + param * d;
    }

    return CoordinateUtils::getDistance(p->latitude / 1000000.0, p->longitude / 1000000.0,
                                        yy / 1000000.0, xx / 1000000.0) / 100.0;


}

std::list <CPoint> *PathSmoothTool::reducerVerticalThreshold(std::list <CPoint> *inPoints, float threshold) {
    std::list <CPoint> *outPoints = reduceNoisePoint(inPoints, threshold);
    inPoints->clear();
    return outPoints;
}

#endif
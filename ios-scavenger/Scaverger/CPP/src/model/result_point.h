//
// 算法输出结果点
// Created by liruopeng on 2023/7/26.
//

#include "string"

#ifndef ANDROID_SCAVENGER_RESULT_POINT_H
#define ANDROID_SCAVENGER_RESULT_POINT_H

using namespace std;

class ResultPoint {
public:
    long long second = 0;
    int meter = 0;
    int pace = 9999;
    int step = 0;
    int altitude = 0;
    int latitude;
    int longitude;
    int calorie = 0;
    int climb = 0;
    int type;

    string toJson() const;

    void fromJson(string json);

};


#endif //ANDROID_SCAVENGER_RESULT_POINT_H

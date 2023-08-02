//
// Created by liruopeng on 2023/7/28.
//

#ifndef ANDROID_SCAVENGER_KM_NODE_H
#define ANDROID_SCAVENGER_KM_NODE_H

#include "string"

using namespace std;

class KmNode {
public:
    unsigned int index = 0;
    unsigned int duration = 0;//距离上个节点耗时
    void fromJson(const string &json);

    string toJson() const;
};


#endif //ANDROID_SCAVENGER_KM_NODE_H

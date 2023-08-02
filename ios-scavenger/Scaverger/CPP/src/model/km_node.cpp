//
// Created by liruopeng on 2023/7/28.
//

#include "km_node.h"
#include "../storage/json/json.h"

void KmNode::fromJson(const string &json) {
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(json, root)) {
        index = root["i"].asInt();
        duration = root["d"].asInt();
    }
    root.clear();
}

string KmNode::toJson() const {
    Json::Value root;
    root["i"] = index;
    root["d"] = duration;
    Json::FastWriter writer;
    string strWrite = writer.write(root);
    root.clear();
    return strWrite;
}

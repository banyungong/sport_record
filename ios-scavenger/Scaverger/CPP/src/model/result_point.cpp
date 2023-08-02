//
// Created by liruopeng on 2023/7/26.
//

#include "result_point.h"
#include "../storage/json/json.h"

void ResultPoint::fromJson(string json) {
    if (json.empty()) {
        return;
    }
    Json::Reader reader;
    Json::Value root;
    root.clear();
    if (reader.parse(json, root)) {
        second = root["a"].asInt();
        meter = root["b"].asInt();
        pace = root["c"].asInt();
        step = root["d"].asInt();
        altitude = root["e"].asInt();
        latitude = root["f"].asInt();
        longitude = root["g"].asInt();
        calorie = root["h"].asInt();
        climb = root["i"].asInt();
        type = root["j"].asInt();
    }
    root.clear();
    json.clear();
}

string ResultPoint::toJson() const {
    Json::Value root;
    root.clear();
    root["a"] = second;
    root["b"] = meter;
    root["c"] = pace;
    root["d"] = step;
    root["e"] = altitude;
    root["f"] = latitude;
    root["g"] = longitude;
    root["h"] = calorie;
    root["i"] = climb;
    root["j"] = type;
    Json::FastWriter writer;
    string strWrite = writer.write(root);
    root.clear();
    return strWrite;
}

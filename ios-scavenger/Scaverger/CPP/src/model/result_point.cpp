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
        second = root["second"].asInt();
        meter = root["meter"].asInt();
        pace = root["pace"].asInt();
        step = root["step"].asInt();
        altitude = root["altitude"].asInt();
        latitude = root["latitude"].asInt();
        longitude = root["longitude"].asInt();
        calorie = root["calorie"].asInt();
        type = root["type"].asInt();
    }
    root.clear();
    json.clear();
}

string ResultPoint::toJson() {
    Json::Value root;
    root.clear();
    root["second"] = second;
    root["meter"] = meter;
    root["pace"] = pace;
    root["step"] = step;
    root["altitude"] = altitude;
    root["latitude"] = latitude;
    root["longitude"] = longitude;
    root["calorie"] = calorie;
    root["type"] = type;
    Json::FastWriter writer;
    string strWrite = writer.write(root);
    root.clear();
    return strWrite;
}

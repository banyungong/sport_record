//
// Created by liruopeng on 2023/7/24.
//

#include "crecord.h"
#include "../storage/json/json.h"

string CRecord::toJson() const {
    Json::Value root;
    root["record_id"] = record_id;
    root["status"] = status;
    root["duration"] = duration;
    root["start_time"] = start_time;
    root["end_time"] = end_time;
    root["mileage"] = mileage;
    root["speed"] = speed;
    root["step"] = step;
    root["calorie"] = calorie;
    root["climb"] = climb;
    root["frequency"] = frequency;
    root["kalman_intensity"] = kalman_intensity;
    root["shake_intensity"] = shake_intensity;
    //公里节点
    Json::Value kmNodeListJson;
    for (auto &kmNode: *kmNodeList) {
        kmNodeListJson.append(kmNode.toJson());
    }
    root["kmNodeList"] = kmNodeListJson;
    //暂停点
    Json::Value pausePointListJson;
    for (auto &pausePoint: *pausePointList) {
        pausePointListJson.append(pausePoint);
    }
    root["pausePointList"] = pausePointListJson;
    Json::FastWriter writer;
    string strWrite = writer.write(root);
    kmNodeListJson.clear();
    pausePointListJson.clear();
    root.clear();
    return strWrite;
}

void CRecord::fromJson(const string &json) {
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(json, root)) {
        record_id = root["record_id"].asInt();
        status = (RecordStatus) root["status"].asInt();
        duration = root["duration"].asInt();
        start_time = root["start_time"].asInt();
        end_time = root["end_time"].asInt();
        mileage = root["mileage"].asInt();
        speed = root["speed"].asInt();
        step = root["step"].asInt();
        calorie = root["calorie"].asInt();
        climb = root["climb"].asInt();
        frequency = root["frequency"].asInt();
        kalman_intensity = root["kalman_intensity"].asInt();
        shake_intensity = root["shake_intensity"].asInt();
        //公里节点
        kmNodeList->clear();
        Json::Value kmNodeListJson = root["kmNodeList"];
        for (int i = 0; i < kmNodeListJson.size(); i++) {
            KmNode kmNode;
            kmNode.fromJson(kmNodeListJson[i].toStyledString());
            kmNodeList->push_back(kmNode);
        }
        kmNodeListJson.clear();
        //暂停点
        pausePointList->clear();
        Json::Value pausePointListJson = root["pausePointList"];
        for (int i = 0; i < pausePointListJson.size(); i++) {
            pausePointList->push_back(pausePointListJson[i].asInt());
        }
        pausePointListJson.clear();
    }
    root.clear();
}

void CRecord::reset() {
    record_id = "";
    status = Idle;
    duration = 0;
    start_time = 0;
    end_time = 0;
    mileage = 0;
    speed = 9999;
    step = 0;
    calorie = 0;
    climb = 0;
    frequency = 1;
    kalman_intensity = 1;
    shake_intensity = 1;
    kmNodeList->clear();
    pausePointList->clear();

}

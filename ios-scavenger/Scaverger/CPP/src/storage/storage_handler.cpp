//
// Created by liruopeng on 2023/7/24.
//

#include "storage_handler.h"
#include "json/json.h"

using namespace std;

void StorageHandler::init(string dir) {
    fileDir = dir;
    pointMMFile->initFile(dir, POINT_NAME, FILE_SIZE * 1024 * 5);
    recordMMFile->initFile(dir, RECORD_NAME, FILE_SIZE * 4);
    dir.clear();
}

list<ResultPoint> *StorageHandler::readPointList() {
    string str = pointMMFile->read();
    str = str.substr(0, str.length() - 1);
    str = "[" + str + "]";
    auto *result_point_list = new list<ResultPoint>;
    if (!str.empty()) {
        Json::Reader reader;
        Json::Value root(Json::arrayValue);
        if (reader.parse(str, root)) {
            for (int i = 0; i < root.size(); i++) {
                ResultPoint point;
                point.fromJson(root[i].toStyledString());
                result_point_list->push_back(point);
            }
        }
        root.clear();
    }
    str.clear();
    return result_point_list;
}

void StorageHandler::writePoint(ResultPoint *point, bool force) {
    if (point != nullptr) {
        point_list->push_back(*point);
    }
    if (point_list->empty()) {
        return;
    }
    if (point_list->size() >= 5 || force) {
        for (auto &p: *point_list) {
            //point转为json
            string str = p.toJson() + ",";
            //写入文件
            if (!str.empty()) {
                pointMMFile->append(str);
            }
        }
        point_list->clear();
    }
}

void StorageHandler::writeRecord(CRecord *record) {
    string str = record->toJson();
    recordMMFile->write(str);
    str.clear();
}

CRecord *StorageHandler::readRecord() {
    string str = recordMMFile->read();
    auto *record = new CRecord();
    if (!str.empty()) {
        record->fromJson(str);
    }
    str.clear();
    return record;
}

void StorageHandler::resetFile(CRecord *record) {
    point_list->clear();
    pointMMFile->close(true);
    pointMMFile->initFile(fileDir, POINT_NAME, FILE_SIZE * 1024 * 5);
    record->reset();
    writeRecord(record);
}

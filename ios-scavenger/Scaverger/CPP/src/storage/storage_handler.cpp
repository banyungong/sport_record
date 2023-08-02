//
// Created by liruopeng on 2023/7/24.
//

#include "storage_handler.h"
#include "json/json.h"

using namespace std;

void StorageHandler::init(string dir, string tag) {
    string pointFileName = tag + "_point_list";
    string recordFileName = tag + "_record";
    pointMMFile->initFile(dir, pointFileName, FILE_SIZE);
    recordMMFile->initFile(dir, recordFileName, FILE_SIZE * 256);
    dir.clear();
    tag.clear();
    pointFileName.clear();
    recordFileName.clear();
}

list<ResultPoint> *StorageHandler::readPointList() {
    string str = pointMMFile->read();
    auto *result_point_list = new list<ResultPoint>;
    if (!str.empty()) {
        str = "[" + str + "]";
        Json::Reader reader;
        Json::Value root;
        root.clear();
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
            string str = p.toJson();
            //写入文件
            pointMMFile->append(str);
        }
        point_list->clear();
    }
}

void StorageHandler::clean() {
    point_list->clear();
    pointMMFile->close();
    recordMMFile->close();
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

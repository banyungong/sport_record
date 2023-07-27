//
// Created by liruopeng on 2023/7/24.
//

#include "storage_handler.h"
#include "json/json.h"

using namespace std;

void StorageHandler::init(string dir, string tag) {
    mmfile->initFile(std::move(dir), std::move(tag), FILE_SIZE);
}

void StorageHandler::append_write() {
    if (point_list->empty()) {
        return;
    }
    for (auto &point: *point_list) {
        //point转为json
        string str = point.toJson();
        //写入文件
        mmfile->append(str);
    }
}

list<ResultPoint> StorageHandler::read() {
    string str = mmfile->read();
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
    return *result_point_list;
}

void StorageHandler::writePoint(ResultPoint *point) {
    point_list->push_back(*point);
    if (point_list->size() >= 5) {
        append_write();
        point_list->clear();
    }
}

void StorageHandler::clean() {
    point_list->clear();
    mmfile->close();
}

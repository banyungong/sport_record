//
// Created by liruopeng on 2023/7/24.
//

#ifndef ANDROID_SCAVENGER_STORAGE_HANDLER_H
#define ANDROID_SCAVENGER_STORAGE_HANDLER_H

#define FILE_SIZE 1024
#define FILE_NAME "scavenger"

#include "string"
#include "list"
#include "../model/result_point.h"
#include "mmap/mmfile.h"

using namespace std;

class StorageHandler {
private:
    MMFile *mmfile = new MMFile();
    list<ResultPoint> *point_list = new list<ResultPoint>;
public:
    /**
     * 初始化文件，如果文件不存在，会创建文件
     * @param dir 文件夹路径
     * @param tag 文件标识
     * @return
     */
    void init(string dir, string tag);

    /**
     * 追加写入数据
     * @param point_list
     * @return
     */
    void append_write();

    void writePoint(ResultPoint *point);

    /**
     * 读取数据
     * @return
     */
    list<ResultPoint> read();

    void clean();

};


#endif //ANDROID_SCAVENGER_STORAGE_HANDLER_H

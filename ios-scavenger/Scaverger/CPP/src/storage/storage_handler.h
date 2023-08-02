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
#include "../model/crecord.h"

using namespace std;

class StorageHandler {
private:
    MMFile *pointMMFile = new MMFile();
    MMFile *recordMMFile = new MMFile();

    list<ResultPoint> *point_list = new list<ResultPoint>;
public:
    /**
     * 初始化文件，如果文件不存在，会创建文件
     * @param dir 文件夹路径
     * @param tag 文件标识
     * @return
     */
    void init(string dir, string tag);


    void writePoint(ResultPoint *point, bool force);

    /**
     * 读取点数据
     * @return
     */
    list<ResultPoint> *readPointList();



    void writeRecord(CRecord *point);

    /**
     * 读取记录汇总数据
     * @return
     */
    CRecord * readRecord();

    void clean();

};


#endif //ANDROID_SCAVENGER_STORAGE_HANDLER_H

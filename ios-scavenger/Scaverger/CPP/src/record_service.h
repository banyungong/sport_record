//
// Created by liruopeng on 2023/7/28.
//

#ifndef ANDROID_SCAVENGER_RECORD_SERVICE_H
#define ANDROID_SCAVENGER_RECORD_SERVICE_H

#include "string"
#include "model/result_point.h"
#include "model/crecord.h"
#include "storage/storage_handler.h"

using namespace std;

class RecordService {
private:
    StorageHandler *storageHandler = new StorageHandler();
    //跑步数据处理
    CRecord *cRecord = new CRecord();

public:
    CRecord *getCRecord() const {
        return cRecord;
    }

    list<ResultPoint> *getResultPointList() const {
        return storageHandler->readPointList();
    }

    void setPath(const string &dir) {
        //
        storageHandler->init(dir);
        cRecord = storageHandler->readRecord();
    }

    /**
     * 重制跑步状态为Idle,清空所有数据
     * 只有当状态为End、Idle、Initialized时才能重置
     */
    int reset();

    /**
     * @param frequency 采样率
     * 只有当状态为Idle时才能调用
     */
    int setFrequency(int frequency);

    /**
     * 开始记录
     * 只有当状态为Initialized才能调用
     */
    int start();

    /**
     * 暂停记录
     * 只有当状态为Running才能调用
     */
    int pause();

    /**
     * 恢复记录
     * 只有当状态为Pause才能调用
     */
    int resume();

    /**
     * 结束记录
     * 只有当状态为Pause才能调用
     */
    int stop();

    /**
     * 添加数据
     */
    void *addPoint(ResultPoint *cPoint);
};


#endif //ANDROID_SCAVENGER_RECORD_SERVICE_H

//
// Created by liruopeng on 2023/7/28.
//

#include "record_service.h"
#include "android/log.h"

int RecordService::reset() {
    if (cRecord->status != RecordStatus::End && cRecord->status != RecordStatus::Idle) {
        return -1;
    }
    cRecord->reset();
    storageHandler->resetFile(cRecord);
    return 0;
}

int RecordService::setFrequency(int f) {
    if (cRecord->status != RecordStatus::Idle) {
        return -1;
    }
    cRecord->frequency = f;
    storageHandler->writeRecord(cRecord);
    return 0;
}

int RecordService::start() {
    if (cRecord->status != RecordStatus::Idle) {
        return -1;
    }
    cRecord->start_time = time(NULL);
    cRecord->status = RecordStatus::Running;
    storageHandler->writeRecord(cRecord);
    return 0;
}

int RecordService::pause() {
    if (cRecord->status != RecordStatus::Running) {
        return -1;
    }
    cRecord->pausePointList->push_back((cRecord->duration / cRecord->frequency) - 1);
    cRecord->status = RecordStatus::Pause;
    //强制存储轨迹点数据
    storageHandler->writePoint(nullptr, true);
    //强制存储跑步数据
    storageHandler->writeRecord(cRecord);
    //增加暂停点
    return 0;
}

int RecordService::resume() {
    if (cRecord->status != RecordStatus::Pause) {
        return -1;
    }
    cRecord->status = RecordStatus::Running;
    return 0;
}

int RecordService::stop() {
    if (cRecord->status != RecordStatus::Pause) {
        return -1;
    }
    cRecord->status = RecordStatus::End;
    //强制存储轨迹点数据
    storageHandler->writePoint(nullptr, true);
    //强制存储跑步数据
    storageHandler->writeRecord(cRecord);
    return 0;
}

void *RecordService::addPoint(ResultPoint *resultPoint) {
    if (cRecord->status == RecordStatus::Running) {
        cRecord->duration += cRecord->frequency;
        resultPoint->second = cRecord->duration;
        //计算跑步数据
        cRecord->mileage += resultPoint->meter;
        if (cRecord->mileage == 0 || cRecord->duration == 0) {
            cRecord->speed = 0;
        } else {
            cRecord->speed = 100000.0 / cRecord->mileage * cRecord->duration;
        }
        cRecord->step += resultPoint->step;
        cRecord->calorie = resultPoint->calorie;
        cRecord->climb = resultPoint->altitude;
        //计算公里节点
        int nextKmNode = (cRecord->kmNodeList->size() + 1) * 100000;
        if (cRecord->mileage >= nextKmNode) {
            auto *kmNode = new KmNode();
            kmNode->index = (cRecord->duration / cRecord->frequency) - 1;
            unsigned int lastKmNodeDuration = 0;
            if (!cRecord->kmNodeList->empty()) {
                lastKmNodeDuration = cRecord->kmNodeList->back().duration;
            }
            kmNode->duration = cRecord->duration - lastKmNodeDuration;
            cRecord->kmNodeList->push_back(*kmNode);
            delete kmNode;
        }
        storageHandler->writePoint(resultPoint, false);
        storageHandler->writeRecord(cRecord);
    }

    return nullptr;
}


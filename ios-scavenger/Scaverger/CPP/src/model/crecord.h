//
// Created by liruopeng on 2023/7/24.
//

#ifndef ANDROID_SCAVENGER_CRECORD_H
#define ANDROID_SCAVENGER_CRECORD_H

#include "list"
#include "result_point.h"
#include "km_node.h"

using namespace std;

//Idle、Initialized、Running、Pause、End
enum RecordStatus {
    Idle,//0
    Running,//1
    Pause,//2
    End//3
};

class CRecord {
public:
    //记录id
    string record_id = "";
    //跑步状态
    RecordStatus status = Idle;
    //总耗时
    int duration = 0;
    //开始时间
    long long start_time = 0L;
    //结束时间
    long long end_time = 0L;
    //总里程
    int mileage = 0;
    //平均速度
    int speed = 9999;
    //总步数
    int step = 0;
    //卡路里
    int calorie = 0;
    //总爬升
    int climb = 0;
    //采样率
    int frequency = 1;
    //轨迹优化指数
    int kalman_intensity = 1;
    //摇步器作弊指数
    int shake_intensity = 1;

    //公里节点
    list<KmNode> *kmNodeList = new list<KmNode>;
    //暂停点
    list<int> *pausePointList = new list<int>;

    string toJson() const;

    void fromJson(const string &json);

    void reset();

};


#endif //ANDROID_SCAVENGER_CRECORD_H

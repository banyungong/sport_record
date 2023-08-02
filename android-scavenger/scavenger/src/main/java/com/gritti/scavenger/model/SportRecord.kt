package com.gritti.scavenger.model

class SportRecord {
    //记录id
    var recordId: String = ""

    //状态
    var status: Int = 0

    //总时长
    var duration: Int = 0

    //开始时间
    var startTime: Long = 0

    //结束时间
    var endTime: Long = 0

    //总里程
    var mileage = 0

    //平均速度
    var speed = 9999

    //总步数
    var step = 0

    //卡路里
    var calorie = 0

    //总爬升
    var climb = 0

    //采集点集合
    var pointList = ArrayList<Point>()

    //暂停点下标集合
    var pausePointIndexList = ArrayList<Point>()

    //公里节点
    var kmNodeList = ArrayList<KMNode>()

}
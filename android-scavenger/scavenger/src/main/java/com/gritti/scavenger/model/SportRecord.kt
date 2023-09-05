package com.gritti.scavenger.model

import android.os.Parcel
import android.os.Parcelable

class SportRecord() : Parcelable {
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

    constructor(parcel: Parcel) : this() {
        recordId = parcel.readString().toString()
        status = parcel.readInt()
        duration = parcel.readInt()
        startTime = parcel.readLong()
        endTime = parcel.readLong()
        mileage = parcel.readInt()
        speed = parcel.readInt()
        step = parcel.readInt()
        calorie = parcel.readInt()
        climb = parcel.readInt()
    }

    override fun writeToParcel(parcel: Parcel, flags: Int) {
        parcel.writeString(recordId)
        parcel.writeInt(status)
        parcel.writeInt(duration)
        parcel.writeLong(startTime)
        parcel.writeLong(endTime)
        parcel.writeInt(mileage)
        parcel.writeInt(speed)
        parcel.writeInt(step)
        parcel.writeInt(calorie)
        parcel.writeInt(climb)
    }

    override fun describeContents(): Int {
        return 0
    }

    companion object CREATOR : Parcelable.Creator<SportRecord> {
        override fun createFromParcel(parcel: Parcel): SportRecord {
            return SportRecord(parcel)
        }

        override fun newArray(size: Int): Array<SportRecord?> {
            return arrayOfNulls(size)
        }
    }

}
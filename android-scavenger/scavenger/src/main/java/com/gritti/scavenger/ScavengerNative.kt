package com.gritti.scavenger

import com.gritti.scavenger.model.Point
import com.gritti.scavenger.model.SportRecord

class ScavengerNative {

    companion object{
        init {
            System.loadLibrary("scavenger")
        }
    }

    external fun addPoint(
        timestamp: Long,
        latitude: Int,
        longitude: Int,
        stepCount: Int,
        gyroscope: Array<LongArray>
    ): LongArray
    /**
     * 配置参数
     * @param trackSmoothIntensity 平滑优化参数
     * @param antiCheatingIntensity 防作弊强度
     * @param dir 运动数据存储目录
     * @param tag 运动数据标签
     */
    external fun config(trackSmoothIntensity: Int, antiCheatingIntensity: Int,dir: String, tag: String)

    /**
     * 重置运动数据
     */
    external fun sportReset(): Int

    /**
     * 运动开始
     */
    external fun sportStart(): Int

    /**
     * 运动恢复
     */
    external fun sportResume(): Int

    /**
     * 运动暂停
     */
    external fun sportPause(): Int

    /**
     * 运动停止
     */
    external fun sportStop(): Int

    /**
     * 获取运动记录
     */
    external fun getRecord(): SportRecord

    /**
     * 获取所有坐标点
     */
    external fun getPointList(): ArrayList<Point>

    /**
     * 给定坐标集合，抽稀出关键点
     */
    external fun pointRarefy(latlngs: Array<IntArray>): Array<IntArray>


}
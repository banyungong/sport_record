package com.gritti.scavenger.collection

import android.content.Context
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager
import java.util.Date


class GyroscopeSensorHandler(context: Context) : SensorEventListener {

    private val NS2S = 1.0f / 1000000000.0f // 将纳秒转化为秒

    private val alpha = 0.8f
    private val eventValues = FloatArray(3)
    private var timestamp: Long = 0

    private val angle = FloatArray(3)

    private var mSensorManager: SensorManager? = null

    private var mContext: Context = context
    private var lastTime: Long = 0

    private var value: Array<LongArray> = arrayOf()

    /**
     * 注册传感器
     */
    fun start() {
        mSensorManager =
            mContext.applicationContext.getSystemService(Context.SENSOR_SERVICE) as SensorManager
        mSensorManager?.registerListener(
            this,
            mSensorManager!!.getDefaultSensor(Sensor.TYPE_GYROSCOPE),
            SensorManager.SENSOR_DELAY_GAME
        )
    }

    fun stop() {
        if (mSensorManager != null) {
            mSensorManager?.unregisterListener(this)
        }
    }

    public fun getValues(): Array<LongArray> {
        val result: Array<LongArray> = value.copyOf()
        value = arrayOf()
        return result
    }

    public fun reset() {
        value = arrayOf()
    }

    override fun onSensorChanged(sensorEvent: SensorEvent) {
        //基础降噪
        eventValues[0] = alpha * eventValues[0] + (1 - alpha) * sensorEvent.values[0]
        eventValues[1] = alpha * eventValues[1] + (1 - alpha) * sensorEvent.values[1]
        eventValues[2] = alpha * eventValues[0] + (1 - alpha) * sensorEvent.values[2]
        val x: Float = sensorEvent.values[0] - eventValues[0]
        val y: Float = sensorEvent.values[1] - eventValues[1]
        val z: Float = sensorEvent.values[2] - eventValues[2]
        //基础降噪结束
        if (timestamp != 0L) {
            // event.timesamp 表示当前的时间，单位是纳秒（1百万分之一毫秒）
            val dT: Float = (sensorEvent.timestamp - timestamp) * NS2S
            angle[0] += x * dT
            angle[1] += y * dT
            angle[2] += z * dT
            // x轴的旋转角度，手机平放桌上，然后绕侧边转动
            var angleX = Math.toDegrees(angle[0].toDouble()).toFloat()
            // y轴的旋转角度，手机平放桌上，然后绕底边转动
            var angleY = Math.toDegrees(angle[1].toDouble()).toFloat()
            // z轴的旋转角度，手机平放桌上，然后水平旋转
            var angleZ = Math.toDegrees(angle[2].toDouble()).toFloat()
            if (angleX > 0 && angleX < 0.001 || angleX < 0 && angleX > -0.001) {
                angleX = 0f
            }
            if (angleY > 0 && angleY < 0.001 || angleY < 0 && angleY > -0.001) {
                angleY = 0f
            }
            if (angleZ > 0 && angleZ < 0.001 || angleZ < 0 && angleZ > -0.001) {
                angleZ = 0f
            }
            if (System.currentTimeMillis() - lastTime > 14) {
                lastTime = System.currentTimeMillis()
                val timestamp = Date().time + (sensorEvent.timestamp - System.nanoTime()) / 1000000L
                value = value.plusElement(longArrayOf((angleX*1000000).toLong(), (angleY*1000000).toLong(), (angleZ*1000000).toLong(),timestamp))
            }
        }
        timestamp = sensorEvent.timestamp
    }

    override fun onAccuracyChanged(sensor: Sensor?, accuracy: Int) {
    }
}
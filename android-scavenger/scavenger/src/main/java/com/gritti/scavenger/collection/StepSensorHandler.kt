package com.gritti.scavenger.collection

import android.content.Context
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager

class StepSensorHandler(context: Context) : SensorEventListener {
    private var mContext: Context = context
    private var sensorManager: SensorManager? = null
    private var stepCount: Int = 0
    private var systemStepCount: Int = -1;


    fun reset() {
        stepCount = 0
        systemStepCount = -1
    }

    fun register() {
        sensorManager =
            mContext.getSystemService(Context.SENSOR_SERVICE) as SensorManager
        sensorManager!!.registerListener(
            this,
            sensorManager!!.getDefaultSensor(Sensor.TYPE_STEP_COUNTER),
            SensorManager.SENSOR_DELAY_GAME
        )
    }

    fun unRegister() {
        sensorManager?.unregisterListener(this)
    }

    fun getStepCount(): Int {
        return stepCount
    }

    override fun onSensorChanged(event: SensorEvent?) {
        event?.let {
            if (it.sensor.type == Sensor.TYPE_STEP_COUNTER) {
                val value = event.values[0].toInt()
                if (systemStepCount != -1) {
                    stepCount += (value - systemStepCount)
                }
                systemStepCount = value
            }
        }
    }

    override fun onAccuracyChanged(sensor: Sensor?, accuracy: Int) {
    }

}
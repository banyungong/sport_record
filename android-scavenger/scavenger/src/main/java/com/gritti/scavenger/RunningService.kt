package com.gritti.scavenger

import android.app.Service
import android.content.Context
import android.content.Intent
import android.os.IBinder
import android.util.Log
import com.gritti.scavenger.controls.TimerControls
import com.gritti.scavenger.notification.ServiceNotification

/**
 * 1、前台服务通知栏
 * 2、GPS数据采集
 */
class RunningService : Service() {

    private val serviceNotification: ServiceNotification by lazy {
        ServiceNotification(this)
    }

    companion object {
        fun start(context: Context) {
            val intent = Intent(context, RunningService::class.java)
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
                context.startForegroundService(intent)
            } else {
                context.startService(intent)
            }
        }

        fun stop(context: Context) {
            val intent = Intent(context, RunningService::class.java)
            context.stopService(intent)
        }
    }

    override fun onBind(intent: Intent?): IBinder? {
        return null
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
        return START_STICKY
    }

    override fun onCreate() {
        super.onCreate()
        //开启前台通知
        serviceNotification.startForegroundWithNotification(null)

        startForeground(ServiceNotification.NOTIFICATION_ID, serviceNotification.notification)
    }

    override fun onDestroy() {
        super.onDestroy()
        serviceNotification.stopForegroundWithNotification()
    }

}
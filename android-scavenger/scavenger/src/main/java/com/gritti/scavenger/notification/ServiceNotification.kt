package com.gritti.scavenger.notification

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.content.Context
import android.os.Build
import android.widget.RemoteViews
import androidx.core.app.NotificationCompat

class ServiceNotification(context: Context) {
    companion object {
        //通知ID,
        const val NOTIFICATION_ID = 1

        //通知渠道ID
        const val NOTIFICATION_CHANNEL_ID = "scavenger_channel_id_01"
    }

    private var notification: Notification? = null
    private var notificationManager: NotificationManager? = null
    private var applicationContext: Context

    init {
        notificationManager =
            context.getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
        applicationContext = context.applicationContext
    }


    /**
     * 创建通知渠道
     */
    private fun createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val channel = NotificationChannel(
                NOTIFICATION_CHANNEL_ID,
                "scavenger_channel_name",
                NotificationManager.IMPORTANCE_HIGH
            )
            channel.description = "Sports Record Notification"
            notificationManager?.createNotificationChannel(channel)
        }
    }

    /**
     * 创建通知
     */
    private fun createNotification(contentView: RemoteViews?) {
        //创建通知渠道
        createNotificationChannel()
        //创建通知
        val builder = NotificationCompat.Builder(applicationContext, NOTIFICATION_CHANNEL_ID)
        if (contentView != null) {
            builder.setCustomContentView(contentView)
        } else {
            builder.setContentTitle("Scavenger")
            builder.setContentText("Scavenger is running")
        }
        builder.priority = NotificationCompat.PRIORITY_HIGH
        builder.setOngoing(true)
        this.notification
    }

    public fun startForegroundWithNotification(contentView: RemoteViews?) {
        if (notification == null) {
            createNotificationChannel()
            createNotification(contentView)
        }
        notificationManager?.notify(NOTIFICATION_ID, notification)
    }

    public fun stopForegroundWithNotification() {
        notificationManager?.cancel(NOTIFICATION_ID)
        notification = null
    }

}
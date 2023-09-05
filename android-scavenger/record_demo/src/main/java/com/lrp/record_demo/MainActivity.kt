package com.lrp.record_demo

import android.os.Bundle
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.gritti.scavenger.ScavengerManager

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        if (ScavengerManager.getInstance(this)
                .getSportRecord().status == 2 || ScavengerManager.getInstance(this)
                .getSportRecord().status == 3
        ) {
            val intent = RecordActivity.newIntent(this)
            startActivity(intent)
        }

        if (ScavengerManager.getInstance(this).getSportRecord().status == 4) {
            //上传记录
        }
    }

    fun startRecord(view: View) {
        ScavengerManager.getInstance(this).resetRecord()
        ScavengerManager.getInstance(this).startRecord()
        val intent = RecordActivity.newIntent(this)
        startActivity(intent)
    }
}
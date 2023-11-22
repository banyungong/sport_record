package com.lrp.record_demo

import android.view.View
import com.lrp.record_demo.base.BaseActivity
import com.lrp.record_demo.databinding.ActivityRecordBinding
import com.lrp.record_demo.vm.RecordVM

class RecordActivity : BaseActivity<RecordVM, ActivityRecordBinding>() {

    override fun getClazz(): Class<RecordVM> {
        return RecordVM::class.java
    }

    override fun layout(): Int {
        return R.layout.activity_record
    }

    override fun initView() {
        dataBinding?.apply {
            btnMap.setOnClickListener {
                //TODO 打开地图
            }
            btnPauseOrContinue.setOnClickListener {
                // 暂停或继续
                viewModel.pauseOrContinue()
            }
            btnStop.setOnClickListener {
                viewModel.finishRecord()
            }

            viewModel.sportRecordLV.observe(this@RecordActivity) {
                tvDistance.text = String.format("%.2fKM", it.mileage / 1000f)
                tvSecond.text = String.format(
                    "%02d:%02d:%02d",
                    it.duration / 3600,
                    it.duration % 3600 / 60,
                    it.duration % 60
                )
                tvSpeed.text = String.format(
                    "%d'%02d\"",
                    it.speed / 60,
                    it.speed % 60
                )
                tvCalorie.text = String.format("%dCal", it.calorie)
                tvClimb.text = String.format("%dm", it.climb)
                tvStep.text = String.format("%dStep", it.step)
                tvStepFrequency.text =
                    String.format("%d/min", (it.step * 60.0 / it.duration).toInt())
            }
            viewModel.isPauseLV.observe(this@RecordActivity) {
                btnPauseOrContinue.text = if (it) {
                    "继续"
                } else {
                    "暂停"
                }
                btnStop.visibility = if (it) {
                    View.VISIBLE
                } else {
                    View.GONE
                }
            }
        }
    }
}
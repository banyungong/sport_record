package com.lrp.record_demo

import android.content.Context
import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.databinding.DataBindingUtil
import androidx.lifecycle.ViewModelProvider
import com.gritti.scavenger.RecordManager
import com.lrp.record_demo.databinding.ActivityRecordBinding
import com.lrp.record_demo.vm.RecordVM

class RecordActivity : AppCompatActivity() {

    private val recordVM: RecordVM by lazy {
        ViewModelProvider.AndroidViewModelFactory.getInstance(application)
            .create(RecordVM::class.java)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val dataBinding =
            DataBindingUtil.setContentView<ActivityRecordBinding>(this, R.layout.activity_record)
        dataBinding.btnMap.setOnClickListener {
            //TODO 打开地图
        }
        dataBinding.btnPauseOrContinue.setOnClickListener {
            // 暂停或继续
            if (RecordManager.getInstance(this).getSportRecord().status == 2) {
                RecordManager.getInstance(this).pauseRecord()
                dataBinding.btnPauseOrContinue.text = "继续"
            } else if (RecordManager.getInstance(this).getSportRecord().status == 3) {
                RecordManager.getInstance(this).resumeRecord()
            }
        }
        dataBinding.tvDistance.text = recordVM.distanceMutableLiveData.value
        dataBinding.tvSecond.text = recordVM.secondMutableLiveData.value
        dataBinding.tvSpeed.text = recordVM.speedMutableLiveData.value
        dataBinding.tvCalorie.text = recordVM.calorieMutableLiveData.value
        dataBinding.tvClimb.text = recordVM.climbMutableLiveData.value
        dataBinding.tvStep.text = recordVM.stepMutableLiveData.value
        dataBinding.tvStepFrequency.text = recordVM.stepFrequencyMutableLiveData.value
    }

    companion object {
        fun newIntent(context: Context): Intent {
            return Intent(context, RecordActivity::class.java)
        }
    }
}
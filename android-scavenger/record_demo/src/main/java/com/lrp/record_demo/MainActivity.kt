package com.lrp.record_demo

import android.Manifest
import android.content.pm.PackageManager
import android.os.Build
import android.widget.Toast
import com.lrp.record_demo.base.BaseActivity
import com.lrp.record_demo.databinding.ActivityMainBinding
import com.lrp.record_demo.vm.MainVM

class MainActivity : BaseActivity<MainVM, ActivityMainBinding>() {

    override fun getClazz(): Class<MainVM> {
        return MainVM::class.java
    }

    override fun layout(): Int {
        return R.layout.activity_main
    }

    override fun initView() {
        dataBinding?.apply {
            startRecord.setOnClickListener {
                if (requestSportPermissions()) {
                    viewModel.startRecord()
                }
            }
        }
        viewModel.checkRecordStatus()
    }

    private fun requestSportPermissions(): Boolean {
        var permissions = arrayOf<String>()
        if (checkSelfPermission(
                Manifest.permission.ACCESS_FINE_LOCATION
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            permissions = permissions.plus(Manifest.permission.ACCESS_FINE_LOCATION)
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q && checkSelfPermission(
                Manifest.permission.ACCESS_BACKGROUND_LOCATION
            ) != PackageManager.PERMISSION_GRANTED
        ) {
//            permissions = permissions.plus(Manifest.permission.ACCESS_BACKGROUND_LOCATION)
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q && checkSelfPermission(
                Manifest.permission.ACTIVITY_RECOGNITION
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            permissions = permissions.plus(Manifest.permission.ACTIVITY_RECOGNITION)
        }
        if (permissions.isNotEmpty()) {
            requestPermissions(permissions, 1000)
        }
        return permissions.isEmpty()
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        if (requestCode == 1000) {
            var allGranted = true
            grantResults.forEach {
                if (it != PackageManager.PERMISSION_GRANTED) {
                    allGranted = false
                }
            }
            if (allGranted) {
                viewModel.startRecord()
            } else {
                Toast.makeText(applicationContext, "缺少权限", Toast.LENGTH_SHORT).show()
            }
        }
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
    }

}
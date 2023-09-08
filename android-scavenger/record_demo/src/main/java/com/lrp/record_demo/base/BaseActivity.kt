package com.lrp.record_demo.base

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.databinding.DataBindingUtil
import androidx.databinding.ViewDataBinding
import androidx.lifecycle.ViewModelProvider

abstract class BaseActivity<T : BaseVM, D : ViewDataBinding> : AppCompatActivity() {
    val viewModel: T by lazy {
        ViewModelProvider.AndroidViewModelFactory.getInstance(application)
            .create(getClazz())
    }

    var dataBinding: D? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        dataBinding = DataBindingUtil.setContentView<D>(this, layout())
        initView()
        viewModel.finishPageLV.observe(this){
            finish()
        }
    }

    abstract fun getClazz(): Class<T>

    abstract fun layout(): Int

    abstract fun initView()

}
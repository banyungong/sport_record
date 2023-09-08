package com.lrp.record_demo.base

import android.app.Application
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

open class BaseVM(application: Application) : AndroidViewModel(application) {

    val finishPageLV = MutableLiveData<Any?>()

    fun runMainThread(block: suspend () -> Unit) {
        viewModelScope.launch(Dispatchers.Main) {
            block()
        }
    }

    fun finishPage() {
        finishPageLV.postValue(null)
    }


}
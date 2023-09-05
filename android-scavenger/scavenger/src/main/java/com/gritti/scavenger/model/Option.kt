package com.gritti.scavenger.model

import android.os.Parcel
import android.os.Parcelable

class Option() : Parcelable {
    var sportFileDir: String = ""
    var sportTag = "scavenger"
    var trackSmoothIntensity = 3
    var antiCheatingIntensity = 3
    var autoLocation = true

    constructor(parcel: Parcel) : this() {
        sportFileDir = parcel.readString().toString()
        sportTag = parcel.readString().toString()
        trackSmoothIntensity = parcel.readInt()
        antiCheatingIntensity = parcel.readInt()
        autoLocation = parcel.readByte() != 0.toByte()
    }

    override fun describeContents(): Int {
        return 0
    }

    override fun writeToParcel(dest: Parcel, flags: Int) {
        dest.writeString(sportFileDir)
        dest.writeString(sportTag)
        dest.writeInt(trackSmoothIntensity)
        dest.writeInt(antiCheatingIntensity)
        dest.writeInt(if (autoLocation) 1 else 0)
    }

    companion object CREATOR : Parcelable.Creator<Option> {
        override fun createFromParcel(parcel: Parcel): Option {
            return Option(parcel)
        }

        override fun newArray(size: Int): Array<Option?> {
            return arrayOfNulls(size)
        }
    }
}
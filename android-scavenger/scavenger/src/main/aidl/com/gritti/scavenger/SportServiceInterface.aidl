// SportServiceInterface.aidl
package com.gritti.scavenger;

import com.gritti.scavenger.model.Option;

// Declare any non-default types here with import statements

interface SportServiceInterface {

    void config(in Option option);

    void resetRecord();

    void startRecord();

    void pauseRecord();

    void resumeRecord();

    void stopRecord();
}
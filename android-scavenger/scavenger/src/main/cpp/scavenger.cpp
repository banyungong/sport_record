#include <jni.h>
#include <string>
#include "src/model/cpoint.h"
#include "src/manager.h"

static Manager *manager = new Manager();

/**
 * @timestamp: current timestamp in millisecond
 * @latitude: current latitude in degree
 * @longitude: current longitude in degree
 * @step: current step count
 * @sensor_value: array of sensor values
 */
extern "C"
JNIEXPORT jlongArray JNICALL
Java_com_gritti_scavenger_ScavengerNative_action(JNIEnv *env, jobject thiz, jlong timestamp,
                                                 jint latitude, jint longitude, jint step,
                                                 jobjectArray sensor_value) {
    auto *cPoint = new CPoint();
    cPoint->timestamp = (timestamp);
    cPoint->latitude = (latitude);
    cPoint->longitude = (longitude);
    cPoint->step = (step);

    int rowCount = env->GetArrayLength(sensor_value);
    long long values[rowCount][4];
    for (int i = 0; i < rowCount; ++i) {
        jlongArray cols = (jlongArray) env->GetObjectArrayElement(sensor_value, i);
        int colCount = env->GetArrayLength(cols);
        if (colCount != 4) {
            continue;
        }
        jlong *colData = env->GetLongArrayElements(cols, nullptr);

        for (int j = 0; j < colCount; ++j) {
            values[i][j] = colData[j];
        }
    }
    cPoint->sensorValue = (values);
    cPoint->sensorSize = (rowCount);

    CPoint *resultPoint = manager->addPoint(cPoint);
    jlongArray result = env->NewLongArray(4);
    jlong *resultData = env->GetLongArrayElements(result, nullptr);
    resultData[0] = resultPoint->timestamp;
    resultData[1] = resultPoint->latitude;
    resultData[2] = resultPoint->longitude;
    resultData[3] = resultPoint->type;
    env->ReleaseLongArrayElements(result, resultData, 0);
    delete cPoint;
//    delete resultPoint;
//    delete resultPoint;
    return result;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_gritti_scavenger_ScavengerNative_clean(JNIEnv *env, jobject thiz) {
    manager->clean();
}
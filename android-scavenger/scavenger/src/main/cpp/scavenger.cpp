#include <jni.h>
#include <string>
#include <list>
#include "../../../../../ios-scavenger/Scaverger/CPP/src/model/cpoint.h"
#include "../../../../../ios-scavenger/Scaverger/CPP/src/manager.h"
#include "../../../../../ios-scavenger/Scaverger/CPP/src/utils/PathSmoothTool.h"

static Manager *manager = new Manager();
static PathSmoothTool *pathSmoothTool = new PathSmoothTool();

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
extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_gritti_scavenger_ScavengerNative_pointRarefy(JNIEnv *env, jobject thiz,
                                                      jobjectArray latlngs) {
    std::list<CPoint> *point_list = new std::list<CPoint>();
    int rowCount = env->GetArrayLength(latlngs);
    for (int i = 0; i < rowCount; ++i) {
        jobjectArray cols = (jobjectArray) env->GetObjectArrayElement(latlngs, i);
        int colCount = env->GetArrayLength(cols);
        if (colCount != 4) {
            continue;
        }
        jint *colData = env->GetIntArrayElements((jintArray) cols, nullptr);
        auto *cPoint = new CPoint();
        cPoint->timestamp = (colData[0]);
        cPoint->latitude = (colData[1]);
        cPoint->longitude = (colData[2]);
        //int 转枚举
        cPoint->type = (CPoint_TYPE) colData[3];
        point_list->push_back(*cPoint);

        env->ReleaseIntArrayElements((jintArray) cols, colData, 0);
    }
    std::list<CPoint> *out_point_list = pathSmoothTool->reducerVerticalThreshold(point_list, 3);



    jobjectArray result = env->NewObjectArray(out_point_list->size(), env->FindClass("[I"),
                                              nullptr);
    int i = 0;
    for (auto it = out_point_list->begin(); it != out_point_list->end(); it++) {
        jintArray cols = env->NewIntArray(4);
        jint *colData = env->GetIntArrayElements(cols, nullptr);
        colData[0] = it->timestamp;
        colData[1] = it->latitude;
        colData[2] = it->longitude;
        colData[3] = it->type;
        env->ReleaseIntArrayElements(cols, colData, 0);
        env->SetObjectArrayElement(result, i, cols);
        i++;
    }
    return result;
}
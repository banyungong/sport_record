#include <jni.h>
#include <string>
#include <list>
#include "../../../../../ios-scavenger/Scaverger/CPP/src/record_service.h"
#include "../../../../../ios-scavenger/Scaverger/CPP/src/manager.h"
#include "../../../../../ios-scavenger/Scaverger/CPP/src/model/cpoint.h"
#include "../../../../../ios-scavenger/Scaverger/CPP/src/utils/PathSmoothTool.h"

static Manager *manager = new Manager();
static RecordService *recordService = new RecordService();

extern "C" {

jlongArray addPoint(JNIEnv *env, jobject thiz, jlong timestamp,
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
        auto cols = (jlongArray) env->GetObjectArrayElement(sensor_value, i);
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

    ResultPoint *resultPoint = manager->addPoint(cPoint);
    recordService->addPoint(resultPoint);
    jlongArray result = env->NewLongArray(9);
    jlong *resultData = env->GetLongArrayElements(result, nullptr);
    resultData[0] = resultPoint->second;
    resultData[1] = resultPoint->latitude;
    resultData[2] = resultPoint->longitude;
    resultData[3] = resultPoint->type;
    resultData[4] = resultPoint->meter / 100;
    resultData[5] = resultPoint->pace;
    resultData[6] = resultPoint->step;
    resultData[7] = resultPoint->altitude;
    resultData[8] = resultPoint->calorie;

    env->ReleaseLongArrayElements(result, resultData, 0);
    delete cPoint;
    return result;
}


void config(JNIEnv *env, jobject thiz,
            jint track_smooth_intensity,
            jint anti_cheating_intensity, jstring dir,
            jstring tag) {
    manager->init(track_smooth_intensity, anti_cheating_intensity);
    const char *dirChars = env->GetStringUTFChars(dir, nullptr);
    const char *tagChars = env->GetStringUTFChars(tag, nullptr);
    recordService->setPath(dirChars, tagChars);
    env->ReleaseStringUTFChars(dir, dirChars);
    env->ReleaseStringUTFChars(tag, tagChars);
}

jobjectArray pointRarefy(JNIEnv *env, jobject thiz,
                         jobjectArray latlngs) {
    auto *pathSmoothTool = new PathSmoothTool();

    auto *point_list = new std::list<CPoint>();
    int rowCount = env->GetArrayLength(latlngs);
    for (int i = 0; i < rowCount; ++i) {
        auto cols = (jobjectArray) env->GetObjectArrayElement(latlngs, i);
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
        point_list->push_back(*cPoint);

        env->ReleaseIntArrayElements((jintArray) cols, colData, 0);
    }
    std::list<CPoint> *out_point_list = pathSmoothTool->reducerVerticalThreshold(point_list, 0.5);
    jobjectArray result = env->NewObjectArray(out_point_list->size(), env->FindClass("[I"),
                                              nullptr);
    int i = 0;
    for (auto &it: *out_point_list) {
        jintArray cols = env->NewIntArray(5);
        jint *colData = env->GetIntArrayElements(cols, nullptr);
        colData[0] = it.timestamp;
        colData[1] = it.latitude;
        colData[2] = it.longitude;
        env->ReleaseIntArrayElements(cols, colData, 0);
        env->SetObjectArrayElement(result, i, cols);
        i++;
    }
    delete pathSmoothTool;
    return result;
}


jint sportStart(JNIEnv *env, jobject thiz) {
    return recordService->start();
}

jint sportReset(JNIEnv *env, jobject thiz) {
    manager->clean();
    return recordService->reset();
}

jint sportResume(JNIEnv *env, jobject thiz) {
    return recordService->resume();
}

jint sportPause(JNIEnv *env, jobject thiz) {
    manager->clean();
    return recordService->pause();
}

jint sportStop(JNIEnv *env, jobject thiz) {
    manager->clean();
    return recordService->stop();
}

jobject getRecord(JNIEnv *env, jobject thiz) {
    auto *record = recordService->getCRecord();
    jclass recordClass = env->FindClass("com/gritti/scavenger/model/SportRecord");
    jmethodID recordConstructor = env->GetMethodID(recordClass, "<init>", "()V");
    jobject recordObject = env->NewObject(recordClass, recordConstructor);

    jfieldID recordIdField = env->GetFieldID(recordClass, "recordId", "Ljava/lang/String;");
    jstring recordId = env->NewStringUTF(record->record_id.c_str());
    env->SetObjectField(recordObject, recordIdField, recordId);

    int status = record->status;
    jfieldID statusField = env->GetFieldID(recordClass, "status", "I");
    env->SetIntField(recordObject, statusField, status);

    jfieldID durationField = env->GetFieldID(recordClass, "duration", "I");
    env->SetIntField(recordObject, durationField, record->duration);

    jfieldID startTimeField = env->GetFieldID(recordClass, "startTime", "J");
    env->SetLongField(recordObject, startTimeField, record->start_time);

    jfieldID endTimeField = env->GetFieldID(recordClass, "endTime", "J");
    env->SetLongField(recordObject, endTimeField, record->end_time);

    jfieldID mileageField = env->GetFieldID(recordClass, "mileage", "I");
    env->SetIntField(recordObject, mileageField, record->mileage);

    jfieldID speedField = env->GetFieldID(recordClass, "speed", "I");
    env->SetIntField(recordObject, speedField, record->speed);

    jfieldID stepField = env->GetFieldID(recordClass, "step", "I");
    env->SetIntField(recordObject, stepField, record->step);

    jfieldID calorieField = env->GetFieldID(recordClass, "calorie", "I");
    env->SetIntField(recordObject, calorieField, record->calorie);

    jfieldID climbField = env->GetFieldID(recordClass, "climb", "I");
    env->SetIntField(recordObject, climbField, record->climb);
    return recordObject;
}

jobject getPointList(JNIEnv *env, jobject thiz) {
    jclass listClass = env->FindClass("java/util/ArrayList");
    jmethodID listConstructor = env->GetMethodID(listClass, "<init>", "()V");
    jobject listObject = env->NewObject(listClass, listConstructor);
    jmethodID addMethod = env->GetMethodID(listClass, "add", "(Ljava/lang/Object;)Z");

    auto *resultPointList = recordService->getResultPointList();
    jclass pointClass = env->FindClass("com/gritti/scavenger/model/Point");
    for (auto &resultPoint: *resultPointList) {
        jmethodID pointConstructor = env->GetMethodID(pointClass, "<init>", "()V");
        jobject pointObject = env->NewObject(pointClass, pointConstructor);
        jfieldID timestampField = env->GetFieldID(pointClass, "timestamp", "J");
        jfieldID latitudeField = env->GetFieldID(pointClass, "latitude", "F");
        jfieldID longitudeField = env->GetFieldID(pointClass, "longitude", "F");
        jfieldID paceField = env->GetFieldID(pointClass, "pace", "I");
        jfieldID stepField = env->GetFieldID(pointClass, "step", "I");
        jfieldID calorieField = env->GetFieldID(pointClass, "calorie", "I");
        jfieldID climbField = env->GetFieldID(pointClass, "climb", "I");
        jfieldID altitudeField = env->GetFieldID(pointClass, "altitude", "F");
        jfieldID typeField = env->GetFieldID(pointClass, "type", "I");
        jfieldID meterField = env->GetFieldID(pointClass, "meter", "I");
        env->SetLongField(pointObject, timestampField, resultPoint.second);
        env->SetFloatField(pointObject, latitudeField, resultPoint.latitude / 1000000.0f);
        env->SetFloatField(pointObject, longitudeField, resultPoint.longitude / 1000000.0f);
        env->SetIntField(pointObject, paceField, resultPoint.pace);
        env->SetIntField(pointObject, stepField, resultPoint.step);
        env->SetIntField(pointObject, calorieField, resultPoint.calorie);
        env->SetIntField(pointObject, climbField, resultPoint.climb);
        env->SetFloatField(pointObject, altitudeField, resultPoint.altitude / 1000000.0f);
        env->SetIntField(pointObject, typeField, resultPoint.type);
        env->SetIntField(pointObject, meterField, resultPoint.meter);
        env->CallBooleanMethod(listObject, addMethod, pointObject);
    }
    return listObject;
}

}


jint RegisterNatives(JNIEnv *env) {
    jclass clazz = env->FindClass("com/gritti/scavenger/ScavengerNative");
    if (clazz == nullptr) {
        return JNI_ERR;
    }
    JNINativeMethod methods[] = {
            {"config",       "(IILjava/lang/String;Ljava/lang/String;)V",  (void *) config},
            {"addPoint",     "(JIII[[J)[J",                                (void *) addPoint},
            {"pointRarefy",  "([[I)[[I",                                   (void *) pointRarefy},
            {"sportStart",   "()I",                                        (void *) sportStart},
            {"sportReset",   "()I",                                        (void *) sportReset},
            {"sportResume",  "()I",                                        (void *) sportResume},
            {"sportPause",   "()I",                                        (void *) sportPause},
            {"sportStop",    "()I",                                        (void *) sportStop},
            {"getRecord",    "()Lcom/gritti/scavenger/model/SportRecord;", (void *) getRecord},
            {"getPointList", "()Ljava/util/ArrayList;",                    (void *) getPointList}
    };
    return env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0]));
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv *env = nullptr;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    RegisterNatives(env);
    return JNI_VERSION_1_6;
}


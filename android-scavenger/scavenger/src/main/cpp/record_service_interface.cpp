//
// Created by liruopeng on 2023/7/28.
//
#include <jni.h>
#include <string>
#include <list>
#include "../../../../../ios-scavenger/Scaverger/CPP/src/record_service.h"

using namespace std;

RecordService *recordService = new RecordService();


extern "C"
JNIEXPORT void JNICALL
Java_com_gritti_scavenger_RecordServiceInterface_setPath(JNIEnv *env, jobject thiz, jstring dir,
                                                         jstring tag) {
    const char *dirChars = env->GetStringUTFChars(dir, nullptr);
    const char *tagChars = env->GetStringUTFChars(tag, nullptr);
    recordService->setPath(dirChars, tagChars);
    env->ReleaseStringUTFChars(dir, dirChars);
    env->ReleaseStringUTFChars(tag, tagChars);

}
extern "C"
JNIEXPORT jint JNICALL
Java_com_gritti_scavenger_RecordServiceInterface_reset(JNIEnv *env, jobject thiz) {
    return recordService->reset();
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_gritti_scavenger_RecordServiceInterface_start(JNIEnv *env, jobject thiz) {
    return recordService->start();
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_gritti_scavenger_RecordServiceInterface_init(JNIEnv *env, jobject thiz, jint frequency) {
    return recordService->init(frequency);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_gritti_scavenger_RecordServiceInterface_pause(JNIEnv *env, jobject thiz) {
    return recordService->pause();
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_gritti_scavenger_RecordServiceInterface_stop(JNIEnv *env, jobject thiz) {
    return recordService->stop();
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_gritti_scavenger_RecordServiceInterface_resume(JNIEnv *env, jobject thiz) {
    return recordService->resume();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_gritti_scavenger_RecordServiceInterface_addPoint(JNIEnv *env, jobject thiz, jint second,
                                                          jint meter, jint pace, jint step,
                                                          jint altitude, jint latitude,
                                                          jint longitude, jint calorie, jint climb,
                                                          jint type) {
    auto *resultPoint = new ResultPoint();
    resultPoint->second = second;
    resultPoint->meter = meter;
    resultPoint->pace = pace;
    resultPoint->step = step;
    resultPoint->altitude = altitude;
    resultPoint->latitude = latitude;
    resultPoint->longitude = longitude;
    resultPoint->calorie = calorie;
    resultPoint->climb = climb;
    resultPoint->type = type;
    recordService->addPoint(resultPoint);
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_gritti_scavenger_RecordServiceInterface_getRecord(JNIEnv *env, jobject thiz) {
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

extern "C"
JNIEXPORT jobject JNICALL
Java_com_gritti_scavenger_RecordServiceInterface_getPointList(JNIEnv *env, jobject thiz) {
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



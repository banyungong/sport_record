//
// Created by liruopeng on 2023/6/25.
//

#include "shake_cheat_handler.h"
//#include <android/log.h>

ShakeCheatHandler::ShakeCheatHandler() {
    cheatSensorHandler = new CheatSensorHandler();
}

ShakeCheatHandler::~ShakeCheatHandler() {
    delete cheatSensorHandler;
}

bool ShakeCheatHandler::onHandler(std::list<CPoint> *point_list, CPoint *inPoint,
                                  ResultPoint *outPoint) {
    long long (*value)[4] = inPoint->sensorValue;

    for (int i = 0; i < inPoint->sensorSize; ++i) {
        float x = value[i][0] / 1000000.0;
        float y = value[i][1] / 1000000.0;
        float z = value[i][2] / 1000000.0;
        long timestamp = value[i][3];
        cheatSensorHandler->addSensorValue(x, y, z, timestamp);
    }
    if (cheatSensorHandler->computerWalkerCheat() >= intensity) {
        outPoint->type = TYPE_SHAKE_CHEAT;
    }
    return false;
}

void ShakeCheatHandler::clean() {
    cheatSensorHandler->clean();
}

//
// Created by liruopeng on 2023/6/25.
// 摇步器作弊
//

#ifndef SCAVENGER_SHAKE_CHEAT_HANDLER_H
#define SCAVENGER_SHAKE_CHEAT_HANDLER_H


#include "../cheat/CheatSensorHandler.h"
#include "handler.h"

class ShakeCheatHandler : public virtual CHandler{
private:
    CheatSensorHandler *cheatSensorHandler;
public:
    ShakeCheatHandler();
    ~ShakeCheatHandler();
    bool onHandler(std::list<CPoint> *point_list,CPoint *point) override;
    void clean() override;
};


#endif //SCAVENGER_SHAKE_CHEAT_HANDLER_H

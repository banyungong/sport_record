//
// Created by liruopeng on 2023/6/28.
//

#ifndef SCAVENGER_DEFAULT_CHEAT_HANDLER_H
#define SCAVENGER_DEFAULT_CHEAT_HANDLER_H


#include "handler.h"

class DefaultCheatHandler : public CHandler {

private:
    CPoint_TYPE checkSpeed();

    CPoint_TYPE checkStep();


    int range_distance;
    int range_step;
    int range_second;

public:
    bool onHandler(std::list<CPoint> *point_list, CPoint *point) override;
    void clean() override;
};


#endif //SCAVENGER_DEFAULT_CHEAT_HANDLER_H

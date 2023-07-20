//
// Created by liruopeng on 2023/6/25.
//

#ifndef SCAVENGER_HANDLER_H
#define SCAVENGER_HANDLER_H

#include "../model/cpoint.h"

class CHandler {
public:
    virtual bool onHandler(std::list<CPoint> *point_list,CPoint *point);
    virtual void clean();
};

#endif //SCAVENGER_HANDLER_H

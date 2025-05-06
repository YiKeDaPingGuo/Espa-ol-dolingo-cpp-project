#ifndef SCHEDULELINE_H
#define SCHEDULELINE_H

#include"gameobjetcs.h"
#include "worddatabase.h"

class ScheduleLine:public GameObjetcs
{
public:
    ScheduleLine();
    void setScheduleOne(WordDatabase mdb,QDialog* dialog);
    void setScheduleTwo(WordDatabase mdb,QDialog* dialog);
    void setScheduleThree(WordDatabase mdb,QDialog* dialog);
    void setScheduleFour(WordDatabase mdb,QDialog* dialog);
};

#endif // SCHEDULELINE_H

#ifndef SCHEDULELINE_H
#define SCHEDULELINE_H

#include <QPushButton>
#include "worddatabase.h"

class ScheduleLine
{
    QStringList record;
    QPushButton* toolButton ;
public:
    ScheduleLine();
    void setSchedule(WordDatabase mdb,QString filename,QVector<bool> m_answerResults,QWidget* dialog);
    //void setScheduleTwo(WordDatabase mdb,QDialog* dialog);
    //void setScheduleThree(WordDatabase mdb,QDialog* dialog);
    //void setScheduleFour(WordDatabase mdb,QDialog* dialog);
    void clear();
};

#endif // SCHEDULELINE_H

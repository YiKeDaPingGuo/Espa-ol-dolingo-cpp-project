#ifndef SENTENCES_H
#define SENTENCES_H

#include <QString>
#include <QStringList>
#include <QWidget>
#include <QRandomGenerator>
#include <QDebug>

#include "worddatabase.h"

class Sentences
{
    int Choose;
    WordDatabase Db;
    QStringList original ;
    QStringList filepath;
    QStringList english;
    QStringList spanish;
    QList<QStringList> outputresult ;
public:
    Sentences();
    void clear();
    void sentenceMaking(int choose,WordDatabase db);
    QString sentenceConnection(int specie);
    void sentenceMakingSimple() ;
    void sentenceMakingDative() ;
    void sentenceMakingQuestion() ;
    QList<QStringList> getOutputresult();
    QStringList getOutputresultAudio();
};

#endif // SENTENCES_H

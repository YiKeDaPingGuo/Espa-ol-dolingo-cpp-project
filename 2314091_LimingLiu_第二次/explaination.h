#ifndef EXPLAINATION_H
#define EXPLAINATION_H
#include <QStringList>
#include <QWidget>
#include "worddatabase.h"
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QFont>
#include <QFontDatabase>
#include <QObject>

class Explaination:public QObject
{
    //点击显示释义
    QPushButton *currentWordsButton = nullptr;
    QLabel *secondWordsLabel = nullptr;
    int clickCount = 0;
public:
    Explaination();
    void setWords(QStringList sentences, QWidget *dialog, int x, int y, int size);
    void clear();
};

#endif // EXPLAINATION_H

#ifndef ICONDISPLAY_H
#define ICONDISPLAY_H

#include "gameobjetcs.h"

class IconDisplay:public GameObjetcs,public QObject
{
    QLabel* currentSentenceLabel = nullptr;  // 记录当前显示的句子标签
public:
    IconDisplay();
    void setSentence(QString sentence, QWidget *dialog,int x,int y,int size);
    void setButton(QString sentence, QWidget *dialog, int x, int y, int length, int width, std::function<void()> onClicked);
    void clear();
    QPushButton* setChoiceButton(QString filename, int x, int y, int length, int width, QWidget *dialog) ;
};

#endif // ICONDISPLAY_H

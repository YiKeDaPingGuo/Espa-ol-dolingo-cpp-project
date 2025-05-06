#ifndef PARTONE_H
#define PARTONE_H

#include "gameobjetcs.h"
#include "explaination.h"
#include "audio.h"
#include "figure.h"
#include "icondisplay.h"
#include "scheduleline.h"

class PartOne:public GameObjetcs,public QObject
{
    QMovie* challengeMovie = nullptr;  // 用于保存当前挑战界面的动画对象
    QLabel* challengeLabel = nullptr;  // 用于保存当前挑战界面的动画标签
    Explaination m_explaination;
    WordDatabase db;
    Audio aud;
    Figure fig;
    IconDisplay ico;
    ScheduleLine sche;

    QLabel* currentSentenceLabel = nullptr;  // 记录当前显示的句子标签
public:
    explicit PartOne(QDialog *dialog, QObject *parent = nullptr);
    void ChallengeOne();
    void ClassDefined();
    void dialogOneFront(std::function<void()> callback);
    void showLilyNextQuestion();
    void onLilyDialogClosed();
    void checkChoiceButton(QList<QStringList>& records);
    void handleAnswerSelection(int index);
    void checkLilyFinalResult();
    QPushButton* setChoiceButton(QString filename, int x, int y, int length, int width, QWidget *dialog);
    void setButton(QString sentence, QWidget *dialog, int x, int y, int length, int width, std::function<void()> onClicked);
    void setSentence(QString sentence, QWidget *dialog,int x,int y,int size);
    void setChallengeMovie(QString filename, int x, int y, QWidget* dialog);
};

#endif // PARTONE_H

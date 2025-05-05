#ifndef GAMEOBJETCS_H
#define GAMEOBJETCS_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QtDebug>
#include <QMediaPlayer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QKeyEvent>
#include <QToolButton>
#include <QDialog>
#include <QPushButton>
#include <functional>
#include <QStringList>
#include <QList>
#include <QRandomGenerator>
#include <algorithm>
#include <QFontDatabase>
#include <QTextCodec>
#include <QVBoxLayout>
#include <QSoundEffect>
#include <QMutex>
#include <QFile>

class GameObjetcs
{
protected:
    //关卡背景图
    QPixmap backgroundPixmap;
    //存储对话框指针
    QDialog *lilyChallengeDialog = nullptr;
    QDialog *oscarChallengeDialog = nullptr;
    QDialog *falstaffChallengeDialog = nullptr;
    QDialog *duoChallengeDialog = nullptr;

    // 是否在主窗口处理事件
    bool isHandlingEventsInMainWidget = true;

    //任务索引
    int m_currentQuestionIndex1 = 0;

    QList<QPushButton*> m_choiceButtonsLily; // 新增成员变量，用于保存按钮指针


    //不同的关卡
    //Parttwo oscarChallenge;


    //是否完成之前的任务
    bool isLilyDone = false;
    bool isOscarDone = false;
    bool isFalstaffDone = false;


    //记录结果
    QVector<bool> m_answerResults1;
    bool LilyAllcorrect = false;

    //记录选择路径
    QList<QStringList> m_allChoiceAnswers1;

    QPushButton* currentButton = nullptr;    // 记录当前显示的按钮
    QStringList m_currentChoices; // 保存当前正确答案
    QString m_correctImagePath;   // 正确答案的图片路径


public:
    GameObjetcs();
};

#endif // GAMEOBJETCS_H

#ifndef WIDGET_H
#define WIDGET_H

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
#include <vector>

#include "worddatabase.h"
#include "audio.h"
#include "explaination.h"
#include "answerchecker.h"
#include "scheduleline.h"
#include "sentences.h"

class Widget : public QWidget
{
    Q_OBJECT

    QLabel label;         // 第一个角色标签
    QLabel Oscarlabel;    // 其他角色标签
    QLabel Lilylabel;
    QLabel Falstafflabel;
    QLabel Duolabel;
    QLabel* currentSentenceLabel = nullptr;  // 记录当前显示的句子标签
    QLabel* challengeLabel = nullptr;  // 用于保存当前挑战界面的动画标签


    QPushButton LilyButton;
    QPushButton OscarButton;
    QPushButton FalstaffButton;
    QPushButton DuoButton;
    QPushButton* ListenButton;
    QPushButton ExitButton;

    QMovie *movie = nullptr;         // 初始化为 nullptr
    QMovie *Oscarmovie = nullptr;
    QMovie *Lilymovie = nullptr;
    QMovie *Falstaffmovie = nullptr;
    QMovie *Duomovie = nullptr;
    QMovie *Lilychallengemovie = nullptr;
    QMovie *Oscarchallengemovie = nullptr;
    QMovie *Falstaffchallengemovie = nullptr;
    QMovie* challengeMovie = nullptr;  // 用于保存当前挑战界面的动画对象

    QTimer *LinMovetimer = nullptr;  // 定时器初始化为 nullptr
    QPoint targetPos;                // 当前点击的目标位置（需扩展为多个角色）
    QPoint cuPos;
    QPixmap mGameBackground;
    QPixmap backgroundPixmap;
    // 播放声音
    QMediaPlayer mediaPlayer;
    QSoundEffect soundEffect;
    QMutex mediaMutex; // 保护 sound 和答题数据

    QList<QPushButton*> m_choiceButtons;
    QList<QPushButton*> m_normalButtons;

    Audio aud1;
    Audio aud2[7];
    Audio aud3;

    ScheduleLine sche1;
    ScheduleLine sche2;
    ScheduleLine sche3;

    WordDatabase db1;
    WordDatabase db2;
    WordDatabase db3;

    Explaination ex1;
    Explaination ex2[7];
    Explaination ex3;

    Sentences sentences;

    AnswerChecker checker2[5];
    AnswerChecker checker3;

    // 存储每个按钮对应的目标位置
    QPoint LilyTargetPos{120, 260};
    QPoint OscarTargetPos{330, 400};
    QPoint FalstaffTargetPos{200, 500};
    QPoint DuoTargetPos{230, 700};

    //存储对话框指针
    QDialog *lilyChallengeDialog = nullptr;
    QDialog *oscarChallengeDialog = nullptr;
    QDialog *falstaffChallengeDialog = nullptr;
    QDialog *duoChallengeDialog = nullptr;

    // 是否在主窗口处理事件
    bool isHandlingEventsInMainWidget = true;

    //点击显示释义
    QPushButton *currentWordsButton = nullptr;
    QLabel *secondWordsLabel = nullptr;
    int clickCount = 0;

    //是否完成之前的任务
    bool isLilyDone = false;
    bool isOscarDone = false;
    bool isFalstaffDone = false;

    //任务索引
    int m_currentQuestionIndex1 = 0;
    int m_currentQuestionIndex2 = 0;
    int m_currentQuestionIndex3 = 0;

    //记录结果
    QVector<bool> m_answerResults1;
    QVector<bool> m_answerResults2;
    QVector<bool> m_answerResults3;
    QVector<bool> m_singleAnswer2;

    //记录选择路径
    QList<QStringList> m_allChoiceAnswers1;//lily
    QList<QStringList> m_allChoiceAnswers2;//oscar



    QStringList m_currentChoices; // Lily保存当前正确答案
    QString m_correctImagePath;   // Lily正确答案的图片路径

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void setMovie(QString filename, QMovie *&m_movie, QLabel &m_label, int x, int y,QWidget * dialog);
    void setChallengeMovie(QString filename,int x, int y,QWidget * dialog);
    void setMovieButton(QString filename, QMovie *&m_movie, QPushButton &m_Button, int x, int y);
    void ChallengeOne();
    void setSentence(QString sentence, QWidget *dialog,int x,int y,int size);
    void setButton(QString sentence, QWidget *dialog, int x, int y, int length, int width,bool isDelete, std::function<void()> onClicked);
    void setButtons(QStringList sentences, int number, QWidget *dialog, QVector<QVector<int>> data, int length, int width, bool isDelete, std::function<void()> onClicked);
    void dialogOneFront(std::function<void()> callback);//同步执行
    void dialogTwoFront(std::function<void()> callback);//同步执行
    void dialogThreeFront(std::function<void()> callback);//同步执行
    void checkChoiceButton(QList<QStringList>& records);
    void handleAnswerSelection(int index);
    void showLilyNextQuestion();
    void checkLilyFinalResult();
    QPushButton* setChoiceButton(QString filename,int x, int y,int length, int width, QWidget * dialog);
    void setSchedule(WordDatabase mdb,QVector<bool> m_answerResults,QWidget* dialog);
    void playSound(QWidget * dialog,QString resourcePath);
    void setWords(QStringList sentence, QWidget *dialog,int x,int y,int size,int spanish);
    void ChallengeTwo();
    void ChallengeThree();
    void showOscarNextLearn();
    void showOscarNextQuestion();
    void showFalstaffNextQuestion();
    void checkOscarFinalResult();
    void checkFalstaffFinalResult();

private slots:
    void moveGif();
    void onButtonClicked(QPushButton* button);
    void onLilyDialogClosed();
    void onOscarDialogClosed();
    void onFalstaffDialogClosed();
};


#endif // WIDGET_H

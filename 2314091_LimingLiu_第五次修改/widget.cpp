#include "widget.h"
#include "worddatabase.h"

#include <QMouseEvent>
#include <QVector2D>
#include <QDebug>
#include <QPushButton>
#include <QMouseEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setGeometry(100, 100, 540, 960);
    setWindowTitle("Dolingo");
    mGameBackground.load(":/png/png/bkground.png");
    if (mGameBackground.load(":/png/png/bkground.png")) {
        QPalette palette = this->palette();
        palette.setBrush(QPalette::Window, QBrush(mGameBackground.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
        this->setPalette(palette);
        this->setAutoFillBackground(true);
    } else {
        qDebug() << "Failed to load background image.";
    }

    setMovie(":/gif/gif/Lin-conducir-izquierda.gif",movie,label,360,100,this);
    setMovieButton(":/gif/gif/Lily-celular.gif", Lilymovie, LilyButton, 120, 260);
    setMovieButton(":/gif/gif/Oscar-tender.gif", Oscarmovie, OscarButton, 330, 400);
    setMovieButton(":/gif/gif/Falstaff-comer.gif", Falstaffmovie, FalstaffButton, 200, 500);
    setMovieButton(":/gif/gif/Duo-la-terria.gif", Duomovie, DuoButton, 230, 700);

    if (movie->state() == QMovie::NotRunning) {
        qDebug() << "Failed to load GIF.";
    }

    LinMovetimer = new QTimer(this);

    connect(LinMovetimer, &QTimer::timeout, this, &Widget::moveGif);
    connect(&LilyButton, &QPushButton::clicked, [this]() { onButtonClicked(&LilyButton); });
    connect(&OscarButton, &QPushButton::clicked, [this]() { onButtonClicked(&OscarButton); });
    connect(&FalstaffButton, &QPushButton::clicked, [this]() { onButtonClicked(&FalstaffButton); });
    connect(&DuoButton, &QPushButton::clicked, [this]() { onButtonClicked(&DuoButton); });
}

Widget::~Widget()
{
    delete movie;
    delete Oscarmovie;
    delete Lilymovie;
    delete Falstaffmovie;
    delete Duomovie;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    int x = event->pos().x() - label.width() / 2;
    int y = event->pos().y() - label.height() / 2;
    targetPos = QPoint(x, y);
    LinMovetimer->start(10); // 启动定时器，每 10 毫秒触发一次
}

void Widget::setMovie(QString filename, QMovie *&m_movie, QLabel &m_label, int x, int y,QWidget * dialog)
{

    m_movie = new QMovie(filename, QByteArray(), dialog);
    m_label.setMovie(m_movie);
    m_label.setFixedSize(108, 108);
    if(m_movie != movie){
        m_label.setFixedSize (250,333);
    }
    m_label.setAlignment(Qt::AlignCenter);
    m_label.setParent(dialog);
    m_label.move(x, y);
    m_label.show();
    m_movie->start();
}

void Widget::setChallengeMovie(QString filename, int x, int y, QWidget* dialog)
{
    // 清理之前的动画
    if (challengeLabel) {
        challengeLabel->deleteLater();
        challengeLabel = nullptr;
    }
    if (challengeMovie) {
        challengeMovie->deleteLater();
        challengeMovie = nullptr;
    }

    // 创建新动画
    challengeLabel = new QLabel(dialog);
    challengeMovie = new QMovie(filename, QByteArray(), dialog);

    challengeLabel->setMovie(challengeMovie);
    challengeLabel->setFixedSize(250, 333);
    challengeLabel->setAlignment(Qt::AlignCenter);
    challengeLabel->move(x, y);
    challengeLabel->show();
    challengeMovie->start();
}


void Widget::setMovieButton(QString filename, QMovie *&m_movie, QPushButton &m_Button, int x, int y)
{
    m_movie = new QMovie(filename, QByteArray(), this);
    m_Button.setFixedSize(108, 108);
    m_Button.setStyleSheet("border: none;");
    m_Button.setParent(this);
    m_Button.move(x, y);
    m_Button.show();

    QLabel *buttonLabel = new QLabel(&m_Button);
    buttonLabel->setMovie(m_movie);
    buttonLabel->setAlignment(Qt::AlignCenter);
    buttonLabel->setFixedSize(108, 108);
    buttonLabel->show();

    m_movie->start();
}

void Widget::ChallengeOne() {
    if (lilyChallengeDialog) {
        isHandlingEventsInMainWidget = false;
        m_currentQuestionIndex1 = 0;
        m_answerResults1.clear();
        m_allChoiceAnswers1.clear();

        dialogOneFront([this](){
            qDebug()<<"寒暄结束";
            //WordDatabase db;
            for (int j = 1;j<=12;j++) {
                QList<QStringList> records = db1.findWithRandomRecords (":/csv/neutral.csv","id",QString::number(j));
                if(records.size()>3){
                    m_allChoiceAnswers1.append(records);
                }
            };
            sche1.setSchedule(db1,":/csv/schedule.csv",m_answerResults1,lilyChallengeDialog);

            //开始第一题答题
            showLilyNextQuestion();
        });
    }
}

void Widget::setSentence(QString sentence, QWidget *dialog,int x,int y,int size)
{
    if(dialog == lilyChallengeDialog){
        setChallengeMovie(":/gif/gif/LilyHablar.gif", 80, 80, dialog);
    }else if(dialog == oscarChallengeDialog){
        setChallengeMovie(":/gif/gif/OscarHablar.gif", 80, 80, dialog);
    }else if(dialog == falstaffChallengeDialog){
        setChallengeMovie(":/gif/gif/FalstaffHablar.gif", 80, 80, dialog);
    }

    // 清理之前的句子（如果有）
    if (currentSentenceLabel) {
        currentSentenceLabel->deleteLater();
        currentSentenceLabel = nullptr;
    }
    qDebug()<<sentence;

    // 创建新标签（原实现）

    currentSentenceLabel = new QLabel(dialog);
    QFont font = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
    font.setFamily("OCR A Extended");
    font.setPointSize(size);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QByteArray utf8Data = codec->fromUnicode(sentence);
    QString utf8Sentence = codec->toUnicode(utf8Data);

    currentSentenceLabel->setFont(font);
    currentSentenceLabel->setText(sentence);
    currentSentenceLabel->setWordWrap(true);
    currentSentenceLabel->setFixedWidth(350);
    currentSentenceLabel->adjustSize();
    currentSentenceLabel->setFixedHeight(currentSentenceLabel->height());
    currentSentenceLabel->move(x, y);
    currentSentenceLabel->show();
}

void Widget::setButtons(QStringList sentences, int number, QWidget *dialog, QVector<QVector<int>> data, int length, int width, bool isDelete, std::function<void()> onClicked)
{
    if (number <= 0 || sentences.size() < number || data.size() < number) {
        // 输入参数不合法，直接返回
        return;
    }

    QTimer::singleShot(1000, [=]() {
        QList<QPushButton*> createdButtons;
        for (int i = 0; i < number; ++i) {
            QString sentence = sentences[i];
            int x = data[i][0];
            int y = data[i][1];

            QPushButton* btn = new QPushButton(sentence, dialog);
            btn->setFont(QFont("OCR A Extended", 15));

            btn->setStyleSheet(
                        "QPushButton {"
                        "   background-color: rgb(166,216,109);"  // 指定背景颜色
                        "   border: none;"                          // 移除边框
                        "   white-space: pre-wrap;"                // 允许自动换行
                        "   text-align: center;"                    // 文字水平居中
                        "   padding: 10px;"                         // 增加内边距
                        "}"
                );

            qDebug() << sentence;

            // 计算文本高度
            QFontMetrics metrics(btn->font());
            int textHeight = metrics.boundingRect(QRect(0, 0, length, 0), Qt::TextWordWrap, sentence).height();
            btn->setFixedSize(length, qMax(width, textHeight + 10));

            btn->move(x, y);
            btn->show();

            createdButtons.append(btn);
            m_normalButtons.append(btn);

            connect(btn, &QPushButton::clicked, [=]() {
                if(btn->text () == "Let's do it right now!"){
                    m_currentQuestionIndex2 = 8;
                }
                QTimer::singleShot(500, [=]() {
                    // 删除当前句子和按钮
                    if (currentSentenceLabel) currentSentenceLabel->deleteLater();
                    if (isDelete) {
                        for (QPushButton* b : createdButtons) {
                            if (b) {
                                b->deleteLater();
                                b = nullptr;
                            }
                        }
                        m_normalButtons.removeAll(btn);
                        currentSentenceLabel = nullptr;
                    }

                    // 执行回调函数（触发下一段对话）
                    if (onClicked) onClicked();
                });
            });
        }
    });
}

void Widget::setButton(QString sentence, QWidget *dialog, int x, int y, int length, int width,bool isDelete, std::function<void()> onClicked)
{

    QTimer::singleShot(1000, [=]() {
        QPushButton* btn = new QPushButton(sentence, dialog);
        btn->setFont(QFont("OCR A Extended", 15));

        btn->setStyleSheet(
                    "QPushButton {"
                    "   background-color: rgb(166,216,109);"  // 指定背景颜色
                    "   border: none;"                          // 移除边框
                    "   white-space: pre-wrap;"                // 允许自动换行
                    "   text-align: center;"                    // 文字水平居中
                    "   padding: 10px;"                         // 增加内边距
                    "}"
            );

        qDebug()<<sentence;

        // 计算文本高度
        QFontMetrics metrics(btn->font());
        int textHeight = metrics.boundingRect(QRect(0, 0, length, 0), Qt::TextWordWrap, sentence).height();
        btn->setFixedSize(length, qMax(width, textHeight + 10));

        //m_normalButtons.append(btn); // 将按钮指针添加到列表中
        btn->move(x, y);
        btn->show();

        connect(btn, &QPushButton::clicked, [=]() {
            QTimer::singleShot(500, [=]() {
                // 删除当前句子和按钮
                if (currentSentenceLabel) currentSentenceLabel->deleteLater();
                if(isDelete){
                    if(isDelete){
                        if (btn) btn->deleteLater();
                        currentSentenceLabel = nullptr;

                    }
                }

                // 执行回调函数（触发下一段对话）
                if (onClicked) onClicked();
            });
        });
    });
}

void Widget::dialogOneFront(std::function<void()> callback)
{
    // 第一段对话
    setChallengeMovie(":/gif/gif/LilyHablar.gif", 80, 80, lilyChallengeDialog);
    aud1.playSpeak (lilyChallengeDialog,"qrc:/aud/Lily/1.wav");
    setSentence("Well,Lin. Want to learn some spanish?", lilyChallengeDialog, 330, 120,15);
    setButton("¡Sí!I want.", lilyChallengeDialog, 150, 700, 400, 150,true, [this,callback]() {
        // 按钮点击后执行的回调
        QTimer::singleShot(4000, [this,callback]() {
            // 恢复等待动画
            setChallengeMovie(":/gif/gif/LilyEsperar.gif", 80, 80, lilyChallengeDialog);
            // 显示第二段对话
            setSentence("Mmm……Good for you, although it is hard.", lilyChallengeDialog, 330, 120,15);
            aud1.playSpeak (lilyChallengeDialog,"qrc:/aud/Lily/2.wav");
            setButton("Haha, let's do it!", lilyChallengeDialog, 150, 700, 400, 150,true, [this,callback]() {
                QTimer::singleShot(500, [this,callback]() {
                    // 恢复等待动画
                    setChallengeMovie(":/gif/gif/LilyEsperar.gif", 80, 80, lilyChallengeDialog);
                    qDebug() << "第一段对话结束。";
                    if (callback) {
                        callback();
                    }
                });
            });
        });
    });
}

void Widget::dialogTwoFront(std::function<void ()> callback)
{
    // 第一段对话
    setChallengeMovie(":/gif/gif/OscarHablar.gif", 80, 80, oscarChallengeDialog);
    aud2[0].playSpeak (oscarChallengeDialog,"qrc:/aud/Oscar/1.wav");
    setSentence("Then I will teach you some verb conjugation.", oscarChallengeDialog, 330, 120,15);
    QStringList sentences = {"Let's do it right now!","I'm ready"};
    QVector<QVector<int>> data = {{150,450},
                                  {150, 700}};

    setButtons(sentences,2,oscarChallengeDialog,data,400,150,true,[this,callback](){
        QTimer::singleShot(4000,[this,callback](){
            setChallengeMovie(":/gif/gif/OscarEsperar.gif", 80, 80, oscarChallengeDialog);
            qDebug() << "第一段对话结束。";
            //m_currentQuestionIndex2 = 8;
            if (callback) {
                callback();
            }
        });
    });

}

void Widget::dialogThreeFront(std::function<void ()> callback)
{
    // 第一段对话
    setChallengeMovie(":/gif/gif/FalstaffHablar.gif", 80, 80, falstaffChallengeDialog);
    aud3.playSpeak (falstaffChallengeDialog,"qrc:/aud/Oscar/1.wav");
    setSentence("Bear says that try to make sentences!", falstaffChallengeDialog, 330, 120,15);
    setButton("Here we go!", falstaffChallengeDialog, 150, 700, 400, 150,true, [this,callback]() {
        // 按钮点击后执行的回调
        QTimer::singleShot(4000, [this,callback]() {
            // 恢复等待动画
            setChallengeMovie(":/gif/gif/FalstaffEsperar.gif", 80, 80, falstaffChallengeDialog);
            qDebug() << "第一段对话结束。";
            if (callback) {
                callback();
            }
        });
    });
}

void Widget::checkChoiceButton(QList<QStringList>& records) {
    QStringList filenames;

    // 提取图片路径（带验证）
    for (int i = 0; i < 4; ++i) {
        int baseIndex = i+m_currentQuestionIndex1*4;
        if (baseIndex < records.size()) {
            filenames.append(records[baseIndex][3]);
        }
    }

    // 验证是否获取到4个选项
    if (filenames.size() != 4) {
        qWarning() << "Failed to get 4 choices";
        return;
    }

    // 记录当前题目的正确答案
    m_currentChoices.append(filenames.first ());

    // 创建按钮（带布局验证）
    QList<QPoint> positions = {
        QPoint(75, 450), QPoint(375, 450),
        QPoint(75, 720), QPoint(375, 720)
    };

    int randomNumber = QRandomGenerator::global()->bounded(4);
    int setpic;

    for (int i = randomNumber; i < 4+randomNumber; ++i) {
        if (i >= randomNumber+positions.size()) break;

        setpic = i%4;//随机打乱正确答案位置

        QPushButton* btn = setChoiceButton(filenames[setpic], positions[i - randomNumber].x(), positions[i - randomNumber].y(), 255, 255, static_cast<QWidget*>(lilyChallengeDialog));

        m_choiceButtons.append(btn); // 将按钮指针添加到列表中
        connect(btn, &QPushButton::clicked, [this, setpic]() {
            handleAnswerSelection(setpic);
        });
    }
}


void Widget::handleAnswerSelection(int index) {
    qDebug() << "Entering handleAnswerSelection with index:" << index;
    QMutexLocker locker(&mediaMutex);
    qDebug() << "Lock acquired in handleAnswerSelection";

    bool isCorrect = (m_allChoiceAnswers1[4*m_currentQuestionIndex1+index][1] == m_currentChoices[1]);
    m_answerResults1.append(isCorrect);
    qDebug() << "Answer result appended. isCorrect:" << isCorrect;

    QString animationPath = isCorrect ? ":/gif/gif/LilyCorrrecto.gif" : ":/gif/gif/LilyCulpa.gif";
    qDebug() << "Animation path set to:" << animationPath;
    setChallengeMovie(animationPath, 80, 80, lilyChallengeDialog);

    // 删除所有按钮
    for (QPushButton* btn : m_choiceButtons) {
        btn->deleteLater();
    }
    m_choiceButtons.clear(); // 清空按钮列表
    ex1.clear();
    QTimer::singleShot(1500, [this]() {
        qDebug() << "Timer callback started in handleAnswerSelection";
        QMutexLocker timerLock(&mediaMutex);
        qDebug() << "Lock acquired in timer callback";
        m_currentQuestionIndex1++;
        qDebug() << "Question index incremented to:" << m_currentQuestionIndex1;
        showLilyNextQuestion();
        qDebug() << "showLilyNextQuestion called in timer callback";
    });
    qDebug() << "Exiting handleAnswerSelection";
}




void Widget::showLilyNextQuestion()
{
    if (!lilyChallengeDialog) {
        qWarning() << "lilyChallengeDialog is null in showLilyNextQuestion";
        return;
    }
    try {
        sche1.setSchedule(db1,":/csv/schedule.csv",m_answerResults1,lilyChallengeDialog);
        setButton("exit", lilyChallengeDialog, 250, 1150, 200, 50,true, [this]() {
            onLilyDialogClosed();
        });
        if (m_currentQuestionIndex1 >= 12 || 4 * m_currentQuestionIndex1 >= m_allChoiceAnswers1.size()) {
            qWarning() << "Index out of bounds in showLilyNextQuestion";
            checkLilyFinalResult();
            return;
        }
        m_currentChoices = m_allChoiceAnswers1[4 * m_currentQuestionIndex1];
        if (m_currentChoices.size() > 5) {
            aud1.clear ();
            aud1.playsound(lilyChallengeDialog, m_currentChoices[5],350,300);
        } else {
            qWarning() << "m_currentChoices size is not enough for playSound in showLilyNextQuestion";
        }
        if (m_currentChoices.size() > 1) {
            ex1.setWords(m_currentChoices, lilyChallengeDialog, 350, 120,20,1,1,270);
        } else {
            qWarning() << "m_currentChoices size is not enough for setSentence in showLilyNextQuestion";
        }

        qDebug() << "现在的题目是" << m_currentChoices[0];

        if (m_currentChoices.size() > 3) {
            qDebug() << "进入check" << m_currentChoices[0];
            checkChoiceButton(m_allChoiceAnswers1);
        }
    } catch (const std::exception& e) {
        qWarning() << "Exception caught in showLilyNextQuestion:" << e.what();
    }
}


void Widget::checkLilyFinalResult()
{
    setChallengeMovie(":/gif/gif/LilyHablar.gif", 80, 80, lilyChallengeDialog);
    if (!m_answerResults1.empty() && std::all_of(m_answerResults1.begin(), m_answerResults1.end(), [](bool val) { return val; })) {
        aud1.playSpeak (lilyChallengeDialog,"qrc:/aud/Lily/3.wav");
        aud1.clearButton ();

        setButton ("Have done the part!", lilyChallengeDialog, 150, 700, 400, 150,true, [this]() {
            QTimer::singleShot(500, [this]() {
                if (lilyChallengeDialog) {
                    isLilyDone = true;
                    lilyChallengeDialog->close();
                    onLilyDialogClosed();

                }
            });
        });
    }else{
        aud1.playSpeak (lilyChallengeDialog,"qrc:/aud/Lily/4.wav");
        aud1.clearButton ();
        setButton ("Try again!", lilyChallengeDialog, 150, 700, 400, 150, true,[this]() {
            QTimer::singleShot(500, [this]() {
                if (lilyChallengeDialog) {
                    isLilyDone = false;
                    lilyChallengeDialog->close();
                    onLilyDialogClosed();
                }
            });
        });
    }
}

QPushButton* Widget::setChoiceButton(QString filename, int x, int y, int length, int width, QWidget *dialog) {
    QPushButton* btn = new QPushButton(dialog);
    btn->setGeometry(QRect(x, y, length, width));

    // 调试：打印按钮属性
    qDebug() << "Button geometry:" << btn->geometry();
    qDebug() << "Parent widget:" << btn->parentWidget();

    // 设置按钮图标（带资源存在性检查）
    qDebug() << "Loading image:" << filename;
    if (QFile::exists(filename)) {
        QPixmap pix(filename);
        if (pix.isNull()) {
            qWarning() << "Pixmap is null for" << filename;
            btn->setText("Image Missing");
        } else {
            btn->setIcon(pix.scaled(255, 255, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            btn->setIconSize(QSize(255, 255));
            btn->setText("");
            btn->setStyleSheet("QPushButton { border: none; background-color: transparent; }");
            btn->setWindowOpacity(1.0);
            qDebug() << "Icon set successfully";
        }
    } else {
        qWarning() << "File not found:" << filename;
        btn->setText("Image Missing");
    }
    btn->show();
    return btn;
}


void Widget::ChallengeTwo()
{
    if (oscarChallengeDialog) {
        isHandlingEventsInMainWidget = false;
        m_currentQuestionIndex2 = 0;
        m_answerResults2.clear();
        m_allChoiceAnswers2.clear();

        dialogTwoFront([this](){

            qDebug()<<"寒暄结束";

            m_allChoiceAnswers2 = db2.loadCSVData (":/csv/oscarVerb.csv");

            if(m_currentQuestionIndex2<8){
                showOscarNextLearn();
            }else{
                showOscarNextQuestion ();
            }
        });
    }
}

void Widget::ChallengeThree()
{
    if (falstaffChallengeDialog) {
        isHandlingEventsInMainWidget = false;
        m_currentQuestionIndex3 = 0;
        m_answerResults3.clear();

        dialogThreeFront([this](){

            qDebug()<<"寒暄结束";

            showFalstaffNextQuestion();
        });
    }
}

void Widget::showOscarNextLearn()
{
    for (int i = 0; i < 7; ++i) {
        ex2[i].clear();
        aud2[i].clear();
    }
    QString sen = "Group "+ QString::number(m_currentQuestionIndex2+1);
    setSentence (sen,oscarChallengeDialog,330, 120,20);
    setChallengeMovie(":/gif/gif/OscarEsperar.gif", 80, 80, oscarChallengeDialog);
    for (int i = 0;i<=6;i++) {
        ex2[i].setWords (m_allChoiceAnswers2[m_currentQuestionIndex2+1],oscarChallengeDialog,350,350+90*i,16,i*2+1,1,270);
        aud2[i].playsound (oscarChallengeDialog,db2.findExactRecord (":/csv/allaudio.csv","spanish",m_allChoiceAnswers2[m_currentQuestionIndex2+1][i*2+1])[1],540,350+90*i);
        if(i == 0)continue;
        ex2[i].setHeader (db2.getHeaderName (":/csv/oscarVerb.csv",i*2+1),oscarChallengeDialog,100,350+90*i,16);
    }
    setButton("exit", oscarChallengeDialog, 370, 1150, 200, 50, true,[this]() {
        ex2->clear ();
        aud2->clear ();
        if (currentSentenceLabel) {
            currentSentenceLabel->deleteLater();
            currentSentenceLabel = nullptr;
        }
        if (oscarChallengeDialog) {
            oscarChallengeDialog->deleteLater();
            oscarChallengeDialog = nullptr;
        }
        onOscarDialogClosed();
    });
    setButton("next group",oscarChallengeDialog,150,1150,200,50,true,[this](){
        m_currentQuestionIndex2++;
        if(m_currentQuestionIndex2<8){
            showOscarNextLearn();
        }else{
            setChallengeMovie(":/gif/gif/OscarHablar.gif", 80, 80, oscarChallengeDialog);
            setSentence("Believe you have already dealed with them, congratulations!",oscarChallengeDialog,330,100,15);
            aud2[0].playSpeak (oscarChallengeDialog,"qrc:/aud/Oscar/4.wav");
            QTimer::singleShot (4000,[=]() {
                ex2->clear ();
                aud2->clear ();
                if (currentSentenceLabel) {
                    currentSentenceLabel->deleteLater();
                    currentSentenceLabel = nullptr;
                }
                if (oscarChallengeDialog) {
                    oscarChallengeDialog->deleteLater();
                    oscarChallengeDialog = nullptr;
                }
            });
        }
    });
}

void Widget::showOscarNextQuestion()
{
    // 遍历 oscarChallengeDialog 的子控件，删除所有 QPushButton
    if (oscarChallengeDialog) {
        QList<QObject*> children = oscarChallengeDialog->children();
        for (QObject* child : children) {
            qDebug()<<"正在删除中";
            QPushButton* button = qobject_cast<QPushButton*>(child);
            if (button) {
                button->deleteLater();
            }
        }
    }
    sche2.setSchedule (db2,":/csv/schedule2.csv",m_answerResults2,oscarChallengeDialog);
    //QString sen = "Group "+ QString::number(m_currentQuestionIndex2-7);
    //setSentence (sen,oscarChallengeDialog,330, 120,20);
    for (int i = 0; i < 7; ++i) {
        ex2[i].clear();
        aud2[i].clear();
    }
    m_singleAnswer2.clear ();
    int array1;
    int array2;
    for(int i = 0; i < 5; i++){
        array1 = QRandomGenerator::global()->bounded(0, 8);
        array2 = QRandomGenerator::global()->bounded(0, 7);
        aud2[i].playsound (oscarChallengeDialog,db2.findExactRecord (":/csv/allaudio.csv","spanish",m_allChoiceAnswers2[array1+1][array2*2+1])[1],320,430+80*i);
        ex2[i].setWords(m_allChoiceAnswers2[array1 + 1], oscarChallengeDialog, 70, 430 + 80 * i, 15, array2 * 2+2, -1,270);
        checker2[i].createInputAndCheck(oscarChallengeDialog, 400, 430 + 80 * i, 100, 50, m_allChoiceAnswers2[array1 + 1][array2 * 2+1],true);
    }


    setButton("next group", oscarChallengeDialog, 150, 1150, 200, 50, false,[this](){
        for (int i = 0; i < 5; ++i) {
             qDebug ()<<"检查第"<<i<<"是否答题"<<checker2[i].getIsChecked ();
            if(!checker2[i].getIsChecked()) {
                setSentence ("Take it easy, complete it first,por favor.", oscarChallengeDialog, 330, 120, 15);
                setChallengeMovie(":/gif/gif/OscarHablar.gif", 80, 80, oscarChallengeDialog);
                qDebug ()<<"检查第"<<i<<"没有答题";
                QTimer::singleShot(4000, [this](){
                    setChallengeMovie(":/gif/gif/OscarEsperar.gif", 80, 80, oscarChallengeDialog);
                    if (currentSentenceLabel) {
                        currentSentenceLabel->deleteLater();
                        currentSentenceLabel = nullptr;
                    }
                });
                return;
            }
            qDebug ()<<"检查第"<<i<<"答题了,存入m_singleAnswer2";
            m_singleAnswer2.append(checker2[i].getResult());
        }
        if(m_singleAnswer2.size() == 5){
            if (!m_singleAnswer2.empty() && std::all_of(m_singleAnswer2.begin(), m_singleAnswer2.end(), [](bool val) { return val; })) {
                m_answerResults2.append (true);
                qDebug()<<"记录全对true";
                setChallengeMovie(":/gif/gif/OscarCorrecto.gif", 80, 80, oscarChallengeDialog);
            }else{
                m_answerResults2.append (false);
                qDebug()<<"记录非全对false";
                setChallengeMovie(":/gif/gif/OscarCulpa.gif", 80, 80, oscarChallengeDialog);
            }
            QTimer::singleShot (4000,[=](){
                m_currentQuestionIndex2++;
                setChallengeMovie(":/gif/gif/OscarEsperar.gif", 80, 80, oscarChallengeDialog);
                sche2.clear ();
                sche2.setSchedule (db2,":/csv/schedule2.csv",m_answerResults2,oscarChallengeDialog);
                for(int i = 0;i<5;i++){
                    checker2[i].clear();
                    qDebug()<<"清理第"<<i<<"个";
                }
                m_singleAnswer2.clear ();
                if(m_currentQuestionIndex2 == 11){
                    checkOscarFinalResult();
                }else{
                    showOscarNextQuestion();
                }
            });

        }
    });
    setButton("exit", oscarChallengeDialog, 370, 1150, 200, 50,true, [this]() {
        onOscarDialogClosed();
    });
}

void Widget::showFalstaffNextQuestion()
{
    //先清理原始数据
    //sentences.clear ();
    ex3.clear ();
    //checker3.clear ();
    aud3.clear();

    //设置进度条
    sche3.setSchedule (db3,":/csv/schedule3.csv",m_answerResults3,falstaffChallengeDialog);
    sentences.sentenceMaking (m_currentQuestionIndex3+5,db3);
    QStringList qanda;
    qanda.clear();
    qanda.append (sentences.sentenceConnection (1));
    qanda.append (sentences.sentenceConnection (2));
    ex3.setWords (qanda,falstaffChallengeDialog,100,500,15,0,1,500);
    checker3.createInputAndCheck (falstaffChallengeDialog,100,600,500,100,qanda[1],false);
    aud3.playSenSound (falstaffChallengeDialog,sentences.getOutputresultAudio (),330,250);
    setButton("next group", falstaffChallengeDialog, 150, 1150, 200, 50, false,[this](){
        qDebug ()<<"检查是否答题"<<checker3.getIsChecked ();
        if(!checker3.getIsChecked ()){
            setSentence ("Take it easy, complete it first,por favor.", falstaffChallengeDialog, 330, 120, 15);
            setChallengeMovie(":/gif/gif/FalstaffHablar.gif", 80, 80, falstaffChallengeDialog);
            return;
        }
        m_answerResults3.append(checker3.getResult ());
        if (m_answerResults3.last ()) {
            qDebug()<<"记录全对true";
            setChallengeMovie(":/gif/gif/FalstaffCorrecto.gif", 80, 80, falstaffChallengeDialog);
        }else{
            qDebug()<<"记录非全对false";
            setChallengeMovie(":/gif/gif/FalstaffCulpa.gif", 80, 80, falstaffChallengeDialog);
        }
        QTimer::singleShot (4000,[=](){
            m_currentQuestionIndex3++;
            m_currentQuestionIndex3 =6;
            setChallengeMovie(":/gif/gif/FalstaffEsperar.gif", 80, 80, falstaffChallengeDialog);
            sche3.clear ();
            sche3.setSchedule (db3,":/csv/schedule.csv",m_answerResults3,falstaffChallengeDialog);
            if(m_currentQuestionIndex3 >= 6){
                checkFalstaffFinalResult();
            }else{
                showFalstaffNextQuestion();
            }
        });
    });
    setButton("exit", falstaffChallengeDialog, 370, 1150, 200, 50,true, [this]() {
        onFalstaffDialogClosed();
    });
}

void Widget::checkOscarFinalResult()
{
    for(int i = 0;i<5;i++){
        checker2[i].clear();
    }
    for(int i = 0;i<7;i++){
        ex2[i].clear ();
        aud2[i].clear ();
    }
    m_singleAnswer2.clear ();
    setChallengeMovie(":/gif/gif/OscarHablar.gif", 80, 80, oscarChallengeDialog);
    if(m_currentQuestionIndex2 == 8){
        setButton ("Finish the study!", oscarChallengeDialog, 150, 700, 400, 150, true,[this]() {
            QTimer::singleShot(500, [this]() {
                if (oscarChallengeDialog) {
                    isOscarDone = false;
                    oscarChallengeDialog->close();
                    onOscarDialogClosed();
                }
            });
        });
    }else{
        if (!m_answerResults2.empty() && std::all_of(m_answerResults2.begin(), m_answerResults2.end(), [](bool val) { return val; })) {
            aud2[0].playSpeak (oscarChallengeDialog,"qrc:/aud/Oscar/2.wav");
            aud2[0].clearButton ();
            setButton ("Have done the part!", oscarChallengeDialog, 150, 700, 400, 150,true, [this]() {
                QTimer::singleShot(500, [this]() {
                    if (oscarChallengeDialog) {
                        isOscarDone = true;
                        oscarChallengeDialog->close();
                        onOscarDialogClosed();

                    }
                });
            });
        }else{
            aud2[0].playSpeak (oscarChallengeDialog,"qrc:/aud/Oscar/3.wav");
            aud2[0].clearButton ();
            setButton ("Try again!", oscarChallengeDialog, 150, 700, 400, 150, true,[this]() {
                QTimer::singleShot(500, [this]() {
                    if (oscarChallengeDialog) {
                        isOscarDone = false;
                        oscarChallengeDialog->close();
                        onOscarDialogClosed();
                    }
                });
            });
        }
    }

}

void Widget::checkFalstaffFinalResult()
{
    checker3.clear();
    aud3.clear ();
    ex3.clear ();
    setChallengeMovie(":/gif/gif/FalstaffHablar.gif", 80, 80, falstaffChallengeDialog);
    if(!m_answerResults3.empty() && std::all_of(m_answerResults3.begin(), m_answerResults3.end(), [](bool val) { return val; })){
        setButton ("Finish the study!", falstaffChallengeDialog, 150, 700, 400, 150, true,[this]() {
            QTimer::singleShot(500, [this]() {
                if (falstaffChallengeDialog) {
                    isFalstaffDone = true;
                    falstaffChallengeDialog->close();
                    onFalstaffDialogClosed();
                }
            });
        });
    }else{
        setButton ("Maybe practice again!", falstaffChallengeDialog, 150, 700, 400, 150, true,[this]() {
            QTimer::singleShot(500, [this]() {
                if (falstaffChallengeDialog) {
                    isFalstaffDone = false;
                    falstaffChallengeDialog->close();
                    onFalstaffDialogClosed();
                }
            });
        });
    }
}

void Widget::moveGif()
{
    QPoint currentPos = label.pos();
    QPoint delta = targetPos - currentPos;

    if (delta.manhattanLength() <= 3) {
        LinMovetimer->stop();
        movie->stop();
        movie->setFileName(":/gif/gif/Lin-quidar.gif");
        movie->start();
        label.move(targetPos);
    } else {
        if (targetPos.x() > currentPos.x() + 90) {
            // 向右走
            movie->stop();
            movie->setFileName(":/gif/gif/Lin-conducir.gif");
            movie->start();
        } else if (targetPos.x() < currentPos.x() + 18) {
            // 向左走
            movie->stop();
            movie->setFileName(":/gif/gif/Lin-conducir-izquierda.gif");
            movie->start();
        }

        QVector2D vecDelta(delta);
        QVector2D step = vecDelta.normalized() * 3;
        QPoint newStep(step.toPoint());
        label.move(currentPos + newStep);
    }
}

void Widget::onButtonClicked(QPushButton* button)
{
    QPoint currentPos = label.pos();
    QPoint targetPos;

    if (button == &LilyButton) {
        targetPos = LilyTargetPos;
    } else if (button == &OscarButton) {
        targetPos = OscarTargetPos;
    } else if (button == &FalstaffButton) {
        targetPos = FalstaffTargetPos;
    } else if (button == &DuoButton) {
        targetPos = DuoTargetPos;
    }

    QPoint distance = currentPos - targetPos;
    if (distance.manhattanLength() <= 100) {
        QDialog *dialog = new QDialog(this);
        if(button == &LilyButton){
            lilyChallengeDialog = dialog;
            dialog->setWindowTitle("Lily's Challenge");
            dialog->resize(702, 1248);

            if (backgroundPixmap.load(":/png/png/bkground1.png")) {
                QPalette palette = dialog->palette();
                palette.setBrush(QPalette::Window, QBrush(backgroundPixmap.scaled(dialog->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
                dialog->setPalette(palette);
                dialog->setAutoFillBackground(true);
            } else {
                qDebug() << "Failed to load background image.";
            }

            connect(lilyChallengeDialog, &QDialog::finished, this, &Widget::onLilyDialogClosed);
            dialog->show();
            ChallengeOne();
        } else if (button == &OscarButton
                   //&&isLilyDone == true
                   ) {
            oscarChallengeDialog = dialog;
            dialog->setWindowTitle("Oscar's Challenge");
            dialog->resize(702, 1248);

            if (backgroundPixmap.load(":/png/png/bkground1.png")) {
                QPalette palette = dialog->palette();
                palette.setBrush(QPalette::Window, QBrush(backgroundPixmap.scaled(dialog->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
                dialog->setPalette(palette);
                dialog->setAutoFillBackground(true);
            } else {
                qDebug() << "Failed to load background image.";
            }

            connect(oscarChallengeDialog, &QDialog::finished, this, &Widget::onOscarDialogClosed);
            dialog->show();
            ChallengeTwo();
        } else if (button == &FalstaffButton
                   //&&isLilyDone == true
                   //&&isOscarDone == true
                   ) {
            falstaffChallengeDialog = dialog;
            dialog->setWindowTitle("Falstaff's Challenge");
            dialog->resize(702, 1248);

            if (backgroundPixmap.load(":/png/png/bkground1.png")) {
                QPalette palette = dialog->palette();
                palette.setBrush(QPalette::Window, QBrush(backgroundPixmap.scaled(dialog->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
                dialog->setPalette(palette);
                dialog->setAutoFillBackground(true);
            } else {
                qDebug() << "Failed to load background image.";
            }

            connect(falstaffChallengeDialog, &QDialog::finished, this, &Widget::onFalstaffDialogClosed);
            dialog->show();
            ChallengeThree();
        } else if (button == &DuoButton) {
            dialog->setWindowTitle("Duo's Challenge");
            dialog->resize(702, 1248);
            dialog->show();
        }
    }
}


void Widget::onLilyDialogClosed()
{
    isHandlingEventsInMainWidget = true;
    // 安全清理按钮列表（对话框的子部件会随对话框销毁自动删除，此处只需清空列表）
    m_normalButtons.clear(); // 直接清空列表，避免残留无效指针

    // 删除对话框（子部件自动销毁，无需手动处理按钮）
    if (lilyChallengeDialog) {
        lilyChallengeDialog->deleteLater();
        lilyChallengeDialog = nullptr;
    }
    m_currentQuestionIndex1 = 0;

    // 重置指针避免野指针
    challengeLabel = nullptr;
    challengeMovie = nullptr;
    currentSentenceLabel = nullptr;
    aud1.clear();
    ex1.clear ();
    db1.clear();
    sche1.clear ();

}


void Widget::onOscarDialogClosed()
{


    if (!isHandlingEventsInMainWidget) {
        isHandlingEventsInMainWidget = true;
    }
    if (!m_normalButtons.empty()) {
        m_normalButtons.clear();
    }
    if (challengeLabel) {
        challengeLabel = nullptr;
    }

    if (challengeMovie) {
        challengeMovie = nullptr;
    }

    if (currentSentenceLabel) {
        currentSentenceLabel->deleteLater();
        currentSentenceLabel = nullptr;
    }


    m_answerResults2.clear();

    m_singleAnswer2.clear();

    m_currentQuestionIndex2 = 0;

    db2.clear();

    for (int i = 0; i < 7; ++i) {
        ex2[i].clear();
        aud2[i].clear();
    }

    sche2.clear();

    if (oscarChallengeDialog) {
        oscarChallengeDialog->deleteLater();
        oscarChallengeDialog = nullptr;
    }
}

void Widget::onFalstaffDialogClosed()
{
    if (!isHandlingEventsInMainWidget) {
        isHandlingEventsInMainWidget = true;
    }
    if (!m_normalButtons.empty()) {
        m_normalButtons.clear();
    }
    if (challengeLabel) {
        challengeLabel = nullptr;
    }

    if (challengeMovie) {
        challengeMovie = nullptr;
    }

    if (currentSentenceLabel) {
        currentSentenceLabel->deleteLater();
        currentSentenceLabel = nullptr;
    }
    sentences.clear ();
    ex3.clear ();
    checker3.clear ();
    aud3.clear();
    sche3.clear ();
    db3.clear ();
    m_currentQuestionIndex3 = 0;
    m_answerResults3.clear ();

    if (falstaffChallengeDialog) {
        falstaffChallengeDialog->deleteLater();
        falstaffChallengeDialog = nullptr;
    }
}

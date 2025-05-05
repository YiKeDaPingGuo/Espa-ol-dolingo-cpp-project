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
            WordDatabase db;
            for (int j = 1;j<=12;j++) {
                QList<QStringList> records = db.findWithRandomRecords (":/csv/neutral.csv","id",QString::number(j));
                if(records.size()>3){
                    m_allChoiceAnswers1.append(records);
                }
            };
            setSchedule(db);

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

void Widget::setButton(QString sentence, QWidget *dialog, int x, int y, int length, int width, std::function<void()> onClicked)
{
    QTimer::singleShot(1000, [=]() {
        QPushButton* btn = new QPushButton(sentence, dialog);
        btn->setFont(QFont("OCR A Extended", 15));

        btn->setStyleSheet(
                    "QPushButton {"
                    "   background-color: rgb(166，216，109);"  // 指定背景颜色
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

        btn->move(x, y);
        btn->show();

        connect(btn, &QPushButton::clicked, [=]() {
            QTimer::singleShot(500, [=]() {
                // 删除当前句子和按钮
                if (currentSentenceLabel) currentSentenceLabel->deleteLater();
                if (btn) btn->deleteLater();
                currentSentenceLabel = nullptr;

                // 执行回调函数（触发下一段对话）
                if (onClicked) onClicked();
            });
        });
    });
}

void Widget::dialogOneFront(std::function<void()> callback)
{
    // 第一段对话
    setChallengeMovie(":/gif/gif/LilyEsperar.gif", 80, 80, lilyChallengeDialog);
    aud.playSpeak (lilyChallengeDialog,"qrc:/aud/Lily/1.wav");
    setSentence("Well,Lin. Want to learn some spanish?", lilyChallengeDialog, 330, 120,15);
    setButton("¡Sí!I want.", lilyChallengeDialog, 150, 700, 400, 150, [this,callback]() {
        // 按钮点击后执行的回调
        QTimer::singleShot(500, [this,callback]() {
            // 恢复等待动画
            setChallengeMovie(":/gif/gif/LilyEsperar.gif", 80, 80, lilyChallengeDialog);
            // 显示第二段对话
            setSentence("Mmm……Good for you, although it is hard.", lilyChallengeDialog, 330, 120,15);
            aud.playSpeak (lilyChallengeDialog,"qrc:/aud/Lily/2.wav");
            setButton("Haha, let's do it!", lilyChallengeDialog, 150, 700, 400, 150, [this,callback]() {
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
    ex.clear();
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
        // 清理所有旧的按钮
        //qDeleteAll(lilyChallengeDialog->findChildren<QPushButton*>());
        WordDatabase mdb;
        setSchedule(mdb);

        setButton("exit", lilyChallengeDialog, 250, 1150, 200, 50, [this]() {
            onLilyDialogClosed();
        });
        if (m_currentQuestionIndex1 >= 12 || 4 * m_currentQuestionIndex1 >= m_allChoiceAnswers1.size()) {
            qWarning() << "Index out of bounds in showLilyNextQuestion";
            checkLilyFinalResult();
            return;
        }
        m_currentChoices = m_allChoiceAnswers1[4 * m_currentQuestionIndex1];
        if (m_currentChoices.size() > 5) {
            aud.clear ();
            aud.playsound(lilyChallengeDialog, m_currentChoices[5]);
        } else {
            qWarning() << "m_currentChoices size is not enough for playSound in showLilyNextQuestion";
        }
        if (m_currentChoices.size() > 1) {
            ex.setWords(m_currentChoices, lilyChallengeDialog, 350, 120,25);
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
        aud.playSpeak (lilyChallengeDialog,"qrc:/aud/Lily/3.wav");
        aud.clearButton ();

        setButton ("Have done the part!", lilyChallengeDialog, 150, 700, 400, 150, [this]() {
            QTimer::singleShot(500, [this]() {
                if (lilyChallengeDialog) {
                    isLilyDone = true;
                    lilyChallengeDialog->close();
                    onLilyDialogClosed();

                }
            });
        });
    }else{
        aud.playSpeak (lilyChallengeDialog,"qrc:/aud/Lily/4.wav");
        aud.clearButton ();
        setButton ("Try again!", lilyChallengeDialog, 150, 700, 400, 150, [this]() {
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

void Widget::setSchedule(WordDatabase mdb) {

    int elementCount = m_answerResults1.size();
    QStringList record = (mdb).findExactRecord(":/csv/schedule.csv","id",QString::number(elementCount));
    qDebug()<<QString::number(elementCount);//输出答题了几道
    if (!record.isEmpty()) {
        QString filename = record[1];
        qDebug()<<filename;
        QPixmap pixmap(filename);
        if (!pixmap.isNull()) {
            QPushButton* toolButton = new QPushButton(static_cast<QWidget*>(lilyChallengeDialog));
            toolButton->setIcon(QIcon(pixmap));
            toolButton->setIconSize(pixmap.size());
            int x = 600 - pixmap.width(); // 右对齐
            int y = 1000;
            toolButton->move(x, y);
            toolButton->setStyleSheet("QPushButton { border: none; background-color: transparent; }");
            toolButton->show();
        } else {
            qWarning() << "无法加载进度条图片:" << filename;
        }
    }
}

void Widget::playSound(QWidget *dialog, QString resourcePath) {
    if (!ListenButton) {
        qWarning() << "ListenButton is null in playSound";
        return;
    }
    ListenButton = new QPushButton(dialog);
    // 设置按钮图标
    QMutexLocker locker(&mediaMutex); // 自动加锁，离开作用域自动解锁
    QPixmap pix(":/png/png/listen.png");
    ListenButton->setIcon(QIcon(pix));
    ListenButton->setIconSize(pix.size());
    ListenButton->setStyleSheet("QPushButton { border: none; background-color: transparent; }");
    qDebug()<<"进入play sound里面";
    // 设置父窗口
    ListenButton->setParent(dialog);
    // 移动按钮到指定位置
    ListenButton->move(350, 200);
    ListenButton->show();
    connect(ListenButton, &QPushButton::clicked, [this, dialog, resourcePath]() {
        // 清除之前的连接（避免重复连接）
        disconnect(ListenButton, &QPushButton::clicked, nullptr, nullptr);

        if (!QFile::exists(resourcePath)) {
            qWarning() << "音频文件不存在:" << resourcePath;
            return;
        }
        soundEffect.setSource(QUrl(resourcePath));

        // 检查资源是否有效
        if (soundEffect.status() == QSoundEffect::Error) {
            qWarning() << "音频资源错误:";
            return;
        }

        soundEffect.play(); // 播放音频
        setChallengeMovie(":/gif/gif/LilyHablar.gif",80, 80, dialog);

        QTimer::singleShot(4000, [this, dialog]() {
            setChallengeMovie(":/gif/gif/LilyEsperar.gif", 80, 80, dialog);
        });
    });
}

void Widget::setWords(QStringList sentences, QWidget *dialog, int x, int y, int size)
{
    if (sentences.size() < 2) {
        qDebug() << "Not enough sentences in the list.";
        return;
    }

    // 清理之前的句子（如果有）
    if (currentWordsButton) {
        delete currentWordsButton;
        currentWordsButton = nullptr;
    }
    if (secondWordsLabel) {
        delete secondWordsLabel;
        secondWordsLabel = nullptr;
    }
    clickCount = 0;

    QString sentence = sentences[1];
    qDebug() << "进入啦"<<sentence;

    // 创建新按钮
    currentWordsButton = new QPushButton(sentence, dialog);
    QFont font = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
    font.setFamily("OCR A Extended");
    font.setPointSize(size);
    font.setUnderline(true);
    currentWordsButton->setFont(font);
    currentWordsButton->setFixedWidth(150);
    currentWordsButton->setStyleSheet("QPushButton { border: none; background-color: transparent; }");
    currentWordsButton->move(x, y);
    currentWordsButton->show();

    // 连接按钮点击信号
    connect(currentWordsButton, &QPushButton::clicked, this, [=](){
        clickCount++;
        if (secondWordsLabel) {
            if (clickCount % 2 == 1) {
                secondWordsLabel->show();
            } else {
                secondWordsLabel->hide();
            }
        }
    });

    // 存储第二个句子
    if (sentences.size() > 2) {
        secondWordsLabel = new QLabel(dialog);
        QFont secondFont = font;
        secondFont.setPointSize(size - 3);
        secondWordsLabel->setFont(secondFont);
        secondWordsLabel->setText(sentences[2]);
        secondWordsLabel->setWordWrap(true);
        secondWordsLabel->setFixedWidth(320);
        secondWordsLabel->adjustSize();
        secondWordsLabel->setFixedHeight(secondWordsLabel->height());
        secondWordsLabel->move(x, y + currentWordsButton->height());
        secondWordsLabel->hide();
    }
}

void Widget::ChallengeTwo()
{

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
        } else if (button == &OscarButton) {
            oscarChallengeDialog = dialog;
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

            connect(oscarChallengeDialog, &QDialog::finished, this, &Widget::onOscarDialogClosed);
            dialog->show();
            ChallengeTwo();
        } else if (button == &FalstaffButton) {
            dialog->setWindowTitle("Falstaff's Challenge");
            dialog->resize(702, 1248);
            dialog->show();
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
    // 自动删除所有子对象（包括动画资源）
    lilyChallengeDialog->deleteLater();
    lilyChallengeDialog = nullptr;

    // 重置指针避免野指针
    challengeLabel = nullptr;
    challengeMovie = nullptr;
    currentSentenceLabel = nullptr;
    currentButton = nullptr;
    aud.clear();
    ex.clear ();

}

void Widget::onOscarDialogClosed()
{

}

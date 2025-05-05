#include "partone.h"


PartOne::PartOne(QDialog *dialog, QObject *parent)
{
    qDebug() << "进入游戏一";
    lilyChallengeDialog = dialog;
    if (lilyChallengeDialog) {
        lilyChallengeDialog->setWindowTitle("Lily's Challenge");
        lilyChallengeDialog->resize(702, 1248);
        if (backgroundPixmap.load(":/png/png/bkground1.png")) {
            QPalette palette = lilyChallengeDialog->palette();
            palette.setBrush(QPalette::Window, QBrush(backgroundPixmap.scaled(lilyChallengeDialog->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
            lilyChallengeDialog->setPalette(palette);
            lilyChallengeDialog->setAutoFillBackground(true);
        } else {
            qDebug() << "Failed to load background image.";
        }

        connect(lilyChallengeDialog, &QDialog::finished, this, &PartOne::onLilyDialogClosed);
        lilyChallengeDialog->show();
        ChallengeOne();
    }
}

void PartOne::ChallengeOne() {
    if (lilyChallengeDialog) {
        isHandlingEventsInMainWidget = false;
        m_currentQuestionIndex1 = 0;
        m_answerResults1.clear();
        m_allChoiceAnswers1.clear();

        //dialogOneFront([this]() {
            qDebug() << "寒暄结束";
            for (int j = 1; j <= 12; j++) {
                QList<QStringList> records = db.findWithRandomRecords(":/csv/neutral.csv", "id", QString::number(j));
                if (records.size() > 3) {
                    m_allChoiceAnswers1.append(records);
                    qDebug()<<m_allChoiceAnswers1[j-1];
                }
            };
            sche.setScheduleOne(db, lilyChallengeDialog);

            // 开始第一题答题
            showLilyNextQuestion();
        //});
    }
}

void PartOne::dialogOneFront(std::function<void()> callback)
{
    QPointer<QWidget> safeDialog(lilyChallengeDialog); // 保护对话框指针

    auto safeCallback = [=]() {
        if (safeDialog && callback) callback();
    };
    // 第一段对话
    fig.setChallengeMovie(":/gif/gif/LilyHablar.gif", 80, 80, lilyChallengeDialog);
    ico.setSentence("Well,Lin. Want to learn some spanish?", lilyChallengeDialog, 300, 120,15);
    ico.setButton("¡Sí!I want.", lilyChallengeDialog, 150, 700, 400, 150, [this,safeCallback,safeDialog,callback]() {
        // 按钮点击后执行的回调
        QTimer::singleShot(500, [=]() {
            // 恢复等待动画
            if(!safeDialog)return;
            fig.setChallengeMovie(":/gif/gif/LilyHablar.gif", 80, 80, lilyChallengeDialog);
            // 显示第二段对话
            ico.setSentence("Mmm……Good for you, although it is hard.", lilyChallengeDialog, 300, 120,15);
            ico.setButton("Haha, let's do it!", lilyChallengeDialog, 150, 700, 400, 150, [this,safeCallback,safeDialog,callback]() {
                QTimer::singleShot(500, [this,safeCallback,callback]() {
                    // 恢复等待动画
                    fig.setChallengeMovie(":/gif/gif/LilyEsperar.gif", 80, 80, lilyChallengeDialog);
                    qDebug() << "第一段对话结束。";
                    if (callback) {
                        safeCallback();
                    }
                });
            });
        });
    });
}

void PartOne::showLilyNextQuestion()
{
    if (!lilyChallengeDialog) {
        qWarning() << "lilyChallengeDialog is null in showLilyNextQuestion";
        return;
    }
    try {
        fig.setChallengeMovie(":/gif/gif/LilyEsperar.gif", 80, 80, lilyChallengeDialog);
        // 清理所有旧的按钮
        //qDeleteAll(lilyChallengeDialog->findChildren<QPushButton*>());
        sche.setScheduleOne(db, lilyChallengeDialog);
//        ico.setButton("exit", lilyChallengeDialog, 250, 1150, 200, 50, [this]() {
//            onLilyDialogClosed();
//        });
        if (m_currentQuestionIndex1 >= 12 || 4 * m_currentQuestionIndex1 >= m_allChoiceAnswers1.size()) {
            qWarning() << "Index out of bounds in showLilyNextQuestion";
            checkLilyFinalResult();
            return;
        }
        m_currentChoices = m_allChoiceAnswers1[4 * m_currentQuestionIndex1];
        if (m_currentChoices.size() > 5) {
            aud.clear();
            aud.playsound(lilyChallengeDialog, m_currentChoices[5]);
        } else {
            qWarning() << "m_currentChoices size is not enough for playSound in showLilyNextQuestion";
        }
        if (m_currentChoices.size() > 1) {
            m_explaination.setWords(m_currentChoices, lilyChallengeDialog, 300, 200, 25);
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

void PartOne::onLilyDialogClosed()
{
    isHandlingEventsInMainWidget = true;
    // 自动删除所有子对象（包括动画资源）
    lilyChallengeDialog->deleteLater();
    lilyChallengeDialog = nullptr;

    fig.clear();
    aud.clear();

    // 重置指针避免野指针
    //currentSentenceLabel = nullptr;
    ico.clear();
    currentButton = nullptr;
}

void PartOne::checkChoiceButton(QList<QStringList>& records) {
    QStringList filenames;

    // 提取图片路径（带验证）
    for (int i = 0; i < 4; ++i) {
        int baseIndex = i + m_currentQuestionIndex1 * 4;
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
    m_currentChoices.append(filenames.first());

    // 创建按钮（带布局验证）
    QList<QPoint> positions = {
        QPoint(75, 450), QPoint(375, 450),
        QPoint(75, 720), QPoint(375, 720)
    };

    int randomNumber = QRandomGenerator::global()->bounded(4);
    int setpic;
    m_choiceButtonsLily.clear();

    for (int i = randomNumber; i < 4 + randomNumber; ++i) {
        if (i >= randomNumber + positions.size()) break;

        setpic = i % 4; // 随机打乱正确答案位置
        QPushButton* btn = ico.setChoiceButton(filenames[setpic], positions[i - randomNumber].x(), positions[i - randomNumber].y(), 255, 255, static_cast<QWidget*>(lilyChallengeDialog));

        m_choiceButtonsLily.append(btn); // 将按钮指针添加到列表中

        connect(btn, &QPushButton::clicked, [this, setpic]() {
            handleAnswerSelection(setpic);
        });
    }
}

void PartOne::handleAnswerSelection(int index) {
    qDebug() << "Entering handleAnswerSelection with index:" << index;
    //QMutexLocker locker(&mediaMutex);
    qDebug() << "Lock acquired in handleAnswerSelection";
    qDebug()<<"m_curr"<<m_currentQuestionIndex1<<"    length"<<m_allChoiceAnswers1.size ();
    bool isCorrect = (m_allChoiceAnswers1[4 * m_currentQuestionIndex1 + index][1] == m_currentChoices[1]);
    m_answerResults1.append(isCorrect);
    qDebug() << "Answer result appended. isCorrect:" << isCorrect;

    QString animationPath = isCorrect ? ":/gif/gif/LilyCorrrecto.gif" : ":/gif/gif/LilyCulpa.gif";
    qDebug() << "Animation path set to:" << animationPath;
    fig.setChallengeMovie(animationPath, 80, 80, lilyChallengeDialog);

    QTimer::singleShot(1500, [this]() {
        qDebug() << "Timer callback started in handleAnswerSelection";
        //QMutexLocker timerLock(&mediaMutex);
        qDebug() << "Lock acquired in timer callback";
        m_currentQuestionIndex1++;
        qDebug() << "Question index incremented to:" << m_currentQuestionIndex1;
        showLilyNextQuestion();
        qDebug() << "showLilyNextQuestion called in timer callback";
    });
    // 删除所有按钮
    for (QPushButton* btn : m_choiceButtonsLily) {
        btn->deleteLater();
    }
    m_choiceButtonsLily.clear(); // 清空按钮列表
    m_explaination.clear();
    qDebug() << "Exiting handleAnswerSelection";
}

void PartOne::checkLilyFinalResult()
{
    aud.clear();
    if (!m_answerResults1.empty() && std::all_of(m_answerResults1.begin(), m_answerResults1.end(), [](bool val) { return val; })) {
        ico.setSentence("Wow, someone finally crawled through basic Spanish.", lilyChallengeDialog, 330, 120, 15);
        QTimer::singleShot(4000, [this]() {
            fig.setChallengeMovie(":/gif/gif/LilyHablar.gif", 80, 80, lilyChallengeDialog);
        });
        fig.setChallengeMovie(":/gif/gif/LilyEsperar.gif", 80, 80, lilyChallengeDialog);
        LilyAllcorrect = true;
        ico.setButton("Have done the part!", lilyChallengeDialog, 150, 700, 400, 150, [this]() {
            QTimer::singleShot(500, [this]() {
                if (lilyChallengeDialog) {
                    isLilyDone = true;
                    lilyChallengeDialog->close();
                    onLilyDialogClosed();
                }
            });
        });
    } else {
        ico.setSentence("Maybe you can not even handle it,try it again.", lilyChallengeDialog, 330, 120, 15);
        QTimer::singleShot(4000, [this]() {
            fig.setChallengeMovie(":/gif/gif/LilyHablar.gif", 80, 80, lilyChallengeDialog);
        });
        fig.setChallengeMovie(":/gif/gif/LilyEsperar.gif", 80, 80, lilyChallengeDialog);
        LilyAllcorrect = false;
        ico.setButton("Try again!", lilyChallengeDialog, 150, 700, 400, 150, [this]() {
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

//void PartOne::setSentence(QString sentence, QWidget *dialog,int x,int y,int size)
//{
//    if (!dialog || !dialog->isVisible()) {  // 新增检查
//        qWarning() << "Invalid dialog in setSentence";
//        return;
//    }
//    if(dialog == lilyChallengeDialog){
//        setChallengeMovie(":/gif/gif/LilyHablar.gif", 80, 80, dialog);
//    }else if(dialog == oscarChallengeDialog){
//        setChallengeMovie(":/gif/gif/OscarHablar.gif", 80, 80, dialog);
//    }else if(dialog == falstaffChallengeDialog){
//        setChallengeMovie(":/gif/gif/FalstaffHablar.gif", 80, 80, dialog);
//    }

//    // 清理之前的句子（如果有）
//    if (currentSentenceLabel) {
//        currentSentenceLabel->deleteLater();
//        currentSentenceLabel = nullptr;
//    }
//    qDebug()<<sentence;

//    // 创建新标签（原实现）

//    currentSentenceLabel = new QLabel(dialog);
//    QFont font = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
//    font.setFamily("OCR A Extended");
//    font.setPointSize(size);

//    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
//    QByteArray utf8Data = codec->fromUnicode(sentence);
//    QString utf8Sentence = codec->toUnicode(utf8Data);

//    currentSentenceLabel->setFont(font);
//    currentSentenceLabel->setText(sentence);
//    currentSentenceLabel->setWordWrap(true);
//    currentSentenceLabel->setFixedWidth(320);
//    currentSentenceLabel->adjustSize();
//    currentSentenceLabel->setFixedHeight(currentSentenceLabel->height());
//    currentSentenceLabel->move(x, y);
//    currentSentenceLabel->show();
//}

//void PartOne::setButton(QString sentence, QWidget *dialog, int x, int y, int length, int width, std::function<void()> onClicked)
//{
//    if (!dialog || !dialog->isVisible()) {  // 新增检查
//        qWarning() << "Invalid dialog in setButton";
//        return;
//    }
//    QTimer::singleShot(1000, [=]() {
//        if (dialog) {
//            QPushButton* btn = new QPushButton(sentence, dialog);
//            btn->setFont(QFont("OCR A Extended", 15));

//            btn->setStyleSheet(
//                        "QPushButton {"
//                        "   background-color: rgb(166,216,109);"  // 指定背景颜色
//                        "   border: none;"                          // 移除边框
//                        "   white-space: pre-wrap;"                // 允许自动换行
//                        "   text-align: center;"                    // 文字水平居中
//                        "   padding: 10px;"                         // 增加内边距
//                        "}"
//                );

//            qDebug() << sentence;

//            // 计算文本高度
//            QFontMetrics metrics(btn->font());
//            int textHeight = metrics.boundingRect(QRect(0, 0, length, 0), Qt::TextWordWrap, sentence).height();
//            btn->setFixedSize(length, qMax(width, textHeight + 10));

//            btn->move(x, y);
//            btn->show();

//            connect(btn, &QPushButton::clicked, [=]() {
//                QTimer::singleShot(500, [=]() {

//                    // 执行回调函数（触发下一段对话）
//                    if (onClicked) onClicked();
//                });
//            });
//        }
//    });
//}

//QPushButton* PartOne::setChoiceButton(QString filename, int x, int y, int length, int width, QWidget *dialog) {
//    QPushButton* btn = new QPushButton(dialog);
//    btn->setGeometry(QRect(x, y, length, width));

//    // 调试：打印按钮属性
//    qDebug() << "Button geometry:" << btn->geometry();
//    qDebug() << "Parent widget:" << btn->parentWidget();

//    // 设置按钮图标（带资源存在性检查）
//    qDebug() << "Loading image:" << filename;
//    QPixmap pix(filename);
//    if (pix.isNull()) {
//        qWarning() << "Pixmap is null for" << filename;
//        btn->setText("Image Missing");
//    } else {
//        btn->setIcon(pix.scaled(255, 255, Qt::KeepAspectRatio, Qt::SmoothTransformation));
//        btn->setIconSize(QSize(255, 255));
//        btn->setText("");
//        btn->setStyleSheet("QPushButton { border: none; background-color: transparent; }");
//        btn->setWindowOpacity(1.0);
//        qDebug() << "Icon set successfully";
//    }

//    btn->show();
//    return btn;
//}

//void PartOne::setChallengeMovie(QString filename, int x, int y, QWidget* dialog)
//{
//    // 清理之前的动画
//    if (challengeLabel) {
//        challengeLabel->deleteLater();
//        challengeLabel = nullptr;
//    }
//    if (challengeMovie) {
//        challengeMovie->deleteLater();
//        challengeMovie = nullptr;
//    }

//    // 创建新动画
//    challengeLabel = new QLabel(dialog);
//    challengeMovie = new QMovie(filename, QByteArray(), dialog);

//    challengeLabel->setMovie(challengeMovie);
//    challengeLabel->setFixedSize(250, 333);
//    challengeLabel->setAlignment(Qt::AlignCenter);
//    challengeLabel->move(x, y);
//    challengeLabel->show();
//    challengeMovie->start();
//}

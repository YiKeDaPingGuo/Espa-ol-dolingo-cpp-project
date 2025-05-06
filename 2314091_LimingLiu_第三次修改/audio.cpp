#include "audio.h"
#include "audio.h"

Audio::Audio(Figure* fig, QObject *parent)
    : QObject(parent), ListenButton(nullptr), widget(fig)
{

}

Audio::~Audio()
{
    clear();
}

void Audio::playsound(QWidget *dialog, QString resourcePath,int x,int y) {
    if (!ListenButton) {
        qWarning() << "ListenButton is null in playSound";
    }
    ListenButton = new QPushButton(dialog);
    // 设置按钮图标
    QMutexLocker locker(&mediaMutex); // 自动加锁，离开作用域自动解锁
    QPixmap pix(":/png/png/listen.png");
    ListenButton->setIcon(QIcon(pix));
    ListenButton->setIconSize(pix.size());
    ListenButton->setStyleSheet("QPushButton { border: none; background-color: transparent; }");
    qDebug() << "进入play sound里面";
    qDebug() << resourcePath;
    // 设置父窗口
    ListenButton->setParent(dialog);
    // 移动按钮到指定位置
    ListenButton->move(x, y);
    ListenButton->show();


    connect(ListenButton, &QPushButton::clicked, [this, dialog, resourcePath]() {
        // 清除之前的连接（避免重复连接）
        //disconnect(ListenButton, &QPushButton::clicked, nullptr, nullptr);

        soundEffect.setSource(QUrl(resourcePath));

        // 检查资源是否有效
        if (soundEffect.status() == QSoundEffect::Error) {
            qWarning() << "音频资源错误:";
            return;
        }
        widget = new Figure();

        soundEffect.play(); // 播放音频

        if (dialog->objectName() == "lilyChallengeDialog") {
            widget->setChallengeMovie(":/gif/gif/LilyHablar.gif", 80, 80, dialog);
        } else if (dialog->objectName() == "oscarChallengeDialog") {
            widget->setChallengeMovie(":/gif/gif/OscarHablar.gif", 80, 80, dialog);
        } else if (dialog->objectName() == "falstaffChallengeDialog") {
            widget->setChallengeMovie(":/gif/gif/FalstaffHablar.gif", 80, 80, dialog);
        }

        QTimer::singleShot(4000, [this, dialog]() {
            if (widget) {
                if (dialog->objectName() == "lilyChallengeDialog") {
                    widget->setChallengeMovie(":/gif/gif/LilyEsperar.gif", 80, 80, dialog);
                } else if (dialog->objectName() == "oscarChallengeDialog") {
                    widget->setChallengeMovie(":/gif/gif/OscarEsperar.gif", 80, 80, dialog);
                } else if (dialog->objectName() == "falstaffChallengeDialog") {
                    widget->setChallengeMovie(":/gif/gif/FalstaffEsperar.gif", 80, 80, dialog);
                }
            }
        });
    });
}


void Audio::clearButton(){
    if(ListenButton){
        ListenButton->deleteLater ();
        ListenButton = nullptr;

    }
}

void Audio::playSpeak(QWidget *dialog, QString resourcePath)
{
    soundEffect.setSource(QUrl(resourcePath));

    // 检查资源是否有效
    if (soundEffect.status() == QSoundEffect::Error) {
        qWarning() << "音频资源错误:";
        return;
    }
    widget = new Figure();

    soundEffect.play(); // 播放音频
    if (dialog->objectName() == "lilyChallengeDialog") {
        widget->setChallengeMovie(":/gif/gif/LilyHablar.gif", 80, 80, dialog);
    } else if (dialog->objectName() == "oscarChallengeDialog") {
        widget->setChallengeMovie(":/gif/gif/OscarHablar.gif", 80, 80, dialog);
    } else if (dialog->objectName() == "falstaffChallengeDialog") {
        widget->setChallengeMovie(":/gif/gif/FalstaffHablar.gif", 80, 80, dialog);
    }

    QTimer::singleShot(4000, [this, dialog]() {
        if (widget) {
            if (dialog->objectName() == "lilyChallengeDialog") {
                widget->setChallengeMovie(":/gif/gif/LilyEsperar.gif", 80, 80, dialog);
            } else if (dialog->objectName() == "oscarChallengeDialog") {
                widget->setChallengeMovie(":/gif/gif/OscarEsperar.gif", 80, 80, dialog);
            } else if (dialog->objectName() == "falstaffChallengeDialog") {
                widget->setChallengeMovie(":/gif/gif/FalstaffEsperar.gif", 80, 80, dialog);
            }
        }
    });
}

void Audio::clear()
{
    if (ListenButton) {
        delete ListenButton;
        ListenButton = nullptr;
    }
    soundEffect.stop();
    soundEffect.setSource(QUrl());
    mediaPlayer.stop();
    mediaPlayer.setMedia(QMediaContent());
}

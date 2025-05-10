#include "audio.h"
#include "audio.h"

Audio::Audio(QObject *parent)
    : QObject(parent), ListenButton(nullptr)
{

}

Audio::~Audio()
{
    clear();
}

void Audio::playsound(QWidget *dialog, QString resourcePath, int x, int y) {
    if (!ListenButton) {
        qWarning() << "ListenButton is null in playSound";
        ListenButton = new QPushButton(dialog);
    }

    // 设置按钮图标
    QMutexLocker locker(&mediaMutex);
    QPixmap pix(":/png/png/listen.png");
    ListenButton->setIcon(QIcon(pix));
    ListenButton->setIconSize(pix.size());
    ListenButton->setStyleSheet("QPushButton { border: none; background-color: transparent; }");

    qDebug() << "进入play sound里面";
    qDebug() << resourcePath;

    // 设置父窗口并显示按钮
    ListenButton->setParent(dialog);
    ListenButton->move(x, y);
    ListenButton->show();

    // 清除之前的连接
    disconnect(ListenButton, nullptr, nullptr, nullptr);
    disconnect(&soundEffect, nullptr, nullptr, nullptr);

    connect(ListenButton, &QPushButton::clicked, [this, resourcePath]() {
        soundEffect.setSource(QUrl(resourcePath));
        soundEffect.play(); // 播放音频
    });


}

void Audio::playimage(QWidget *dialog, QString resourcePath, QPushButton* btn) {
    if (!btn) {
        qWarning() << "ListenButton is null in playSound";
        btn = new QPushButton(dialog);
    }

    // 设置按钮图标
    QMutexLocker locker(&mediaMutex);

    qDebug() << "进入play sound里面";
    qDebug() << resourcePath;

    // 设置父窗口并显示按钮
    btn->setParent(dialog);

    // 清除之前的连接
    disconnect(btn, nullptr, nullptr, nullptr);
    disconnect(&soundEffect, nullptr, nullptr, nullptr);

    connect(btn, &QPushButton::clicked, [this, resourcePath]() {
        soundEffect.setSource(QUrl(resourcePath));
        soundEffect.play(); // 播放音频
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

    soundEffect.play(); // 播放音频



}

void Audio::clear()
{
    if (ListenButton) {
        delete ListenButton;
        ListenButton = nullptr;
    }

    soundEffect.stop();
    soundEffect.setSource(QUrl());

}

void Audio::playSenSound(QWidget *dialog, QStringList resourcePaths, int x, int y) {
    if (!ListenButton) {
        qWarning() << "ListenButton is null in playSound";
        ListenButton = new QPushButton(dialog);
    }

    // 重置按钮状态
    disconnect(ListenButton, nullptr, nullptr, nullptr);
    ListenButton->disconnect();

    // 设置按钮图标
    QMutexLocker locker(&mediaMutex); // 自动加锁，离开作用域自动解锁
    QPixmap pix(":/png/png/listen.png");
    ListenButton->setIcon(QIcon(pix));
    ListenButton->setIconSize(pix.size());
    ListenButton->setStyleSheet("QPushButton { border: none; background-color: transparent; }");

    qDebug() << "进入play sound里面";
    qDebug() << resourcePaths;

    // 设置父窗口并显示按钮
    ListenButton->setParent(dialog);
    ListenButton->move(x, y);
    ListenButton->show();

    // 如果没有音频文件，直接返回
    if (resourcePaths.isEmpty()) {
        qWarning() << "没有要播放的音频文件";
        return;
    }

    // 创建播放器和播放列表
    QMediaPlayer * player = new QMediaPlayer(this);
    QMediaPlaylist * playlist = new QMediaPlaylist(player);

    // 将所有音频文件添加到播放列表
    for (const QString &path : resourcePaths) {
        playlist->addMedia(QUrl(path));
    }

    // 设置播放模式为顺序播放
    playlist->setPlaybackMode(QMediaPlaylist::Sequential);
    player->setPlaylist(playlist);

    // 记录当前对话框类型
    QString dialogType = dialog->objectName();


    connect(ListenButton, &QPushButton::clicked, [=]() {
        // 开始播放第一个音频
        player->play();
    });


    // 监听错误
    connect(player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
            [](QMediaPlayer::Error error) {
        qWarning() << "播放错误:" << error;
    });

    // 清理资源
    connect(playlist, &QMediaPlaylist::currentIndexChanged, [=](int index) {
        if (index == -1) { // 播放列表结束
            qDebug() << "所有音频播放完毕";
        }
    });
}


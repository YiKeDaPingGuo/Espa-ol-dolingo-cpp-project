#ifndef AUDIO_H
#define AUDIO_H

#include <QObject>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QMutex>
#include <QPushButton>
#include <QResource>
#include <QTimer>

#include "figure.h"

class Audio : public QObject
{
    Q_OBJECT
    QPushButton* ListenButton;
    Figure* widget;
    QMediaPlayer mediaPlayer;
    QSoundEffect soundEffect;
    QMutex mediaMutex; // 保护 sound 和答题数据

public:
    explicit Audio(Figure* fig = nullptr, QObject *parent = nullptr);
    ~Audio();
    void playsound(QWidget *dialog, QString resourcePath);
    void clear();
    void clearButton();
    void playSpeak(QWidget* dialog, QString resourcePath);
};

#endif // AUDIO_H

#ifndef AUDIO_H
#define AUDIO_H

#include <QObject>
#include <QMediaPlayer>
#include <QSoundEffect>
#include <QMutex>
#include <QPushButton>
#include <QResource>
#include <QTimer>
#include <QMediaPlaylist>


class Audio : public QObject
{
    Q_OBJECT
    QPushButton* ListenButton;
    QSoundEffect soundEffect;
    QMutex mediaMutex; // 保护 sound 和答题数据

public:
    explicit Audio(QObject *parent = nullptr);
    ~Audio();
    void playsound(QWidget *dialog, QString resourcePath,int x,int y);
    void clear();
    void clearButton();
    void playSpeak(QWidget* dialog, QString resourcePath);
    void playSenSound(QWidget *dialog, QStringList resourcePaths, int x, int y) ;
    void playimage(QWidget *dialog, QString resourcePath, QPushButton* btn);

};

#endif // AUDIO_H

#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#endif // GAMEOBJECTS_H

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

class gameObjects : public QObject
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
};

#endif // GAMEOBJECTS_H

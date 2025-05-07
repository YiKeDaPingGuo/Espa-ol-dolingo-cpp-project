#include "figure.h"

Figure::Figure()
{
    // 初始化指针为 nullptr
    challengeLabel = nullptr;
    challengeMovie = nullptr;
}

void Figure::setChallengeMovie(QString filename, int x, int y, QWidget* dialog)
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

void Figure::clear()
{
    if (challengeLabel) {
        delete challengeLabel;
        challengeLabel = nullptr;
    }
    if (challengeMovie) {
        delete challengeMovie;
        challengeMovie = nullptr;
    }
}

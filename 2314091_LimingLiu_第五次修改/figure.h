#ifndef FIGURE_H
#define FIGURE_H

#include <QObject>
#include <QLabel>
#include <QMovie>

class Figure
{
    QMovie* challengeMovie = nullptr;  // 用于保存当前挑战界面的动画对象
    QLabel* challengeLabel = nullptr;  // 用于保存当前挑战界面的动画标签
public:
    Figure();
    void setChallengeMovie(QString filename, int x, int y, QWidget* dialog);
    void clear();
};

#endif // FIGURE_H

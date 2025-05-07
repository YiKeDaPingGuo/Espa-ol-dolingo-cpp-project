#ifndef ANSWERCHECKER_H
#define ANSWERCHECKER_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QPixmap>
#include <QDebug>

class AnswerChecker:public QObject
{
    QPushButton* checkButton;
    bool isChecked = false;
    bool result;
    QPixmap pixmap;
    QIcon icon;
    QLineEdit* inputBox;
public:
    AnswerChecker();
    void createInputAndCheck(QDialog* dialog, int x, int y, int length, int width, const QString& correctAnswer);
    void dispCheckAnswer(bool answer,int x,int y);
    QVector<bool> getResults();
    void clear();
    bool getResult();
    bool getIsChecked();
};

#endif // ANSWERCHECKER_H

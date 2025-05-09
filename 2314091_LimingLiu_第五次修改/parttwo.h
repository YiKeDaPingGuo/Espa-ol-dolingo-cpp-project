#ifndef PARTTWO_H
#define PARTTWO_H

#include "gameobjetcs.h"
#include "figure.h"
#include "icondisplay.h"



class Parttwo : public QObject,GameObjetcs
{
    Q_OBJECT
    Figure fig;
    IconDisplay ico;
public:
    explicit Parttwo(QDialog *dialog, QObject *parent = nullptr);

private slots:
    void onOscarDialogClosed();

private:

    QPixmap backgroundPixmap;
};

#endif // PARTTWO_H

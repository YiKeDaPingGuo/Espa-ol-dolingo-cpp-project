#include "parttwo.h"
#include <QDebug>

Parttwo::Parttwo(QDialog *dialog, QObject *parent) : QObject(parent)
{
    oscarChallengeDialog = dialog;
    if (oscarChallengeDialog) {
        oscarChallengeDialog->setWindowTitle("Oscar's Challenge");
        oscarChallengeDialog->resize(702, 1248);
        if (backgroundPixmap.load(":/png/png/bkground1.png")) {
            QPalette palette = oscarChallengeDialog->palette();
            palette.setBrush(QPalette::Window, QBrush(backgroundPixmap.scaled(oscarChallengeDialog->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
            oscarChallengeDialog->setPalette(palette);
            oscarChallengeDialog->setAutoFillBackground(true);
        } else {
            qDebug() << "Failed to load background image.";
        }

        connect(oscarChallengeDialog, &QDialog::finished, this, &Parttwo::onOscarDialogClosed);
        oscarChallengeDialog->show();
    }
    fig.setChallengeMovie(":/gif/gif/OscarEsperar.gif",80,80,oscarChallengeDialog);
    ico.setSentence ("Seem you have learnt some Spanish words.",oscarChallengeDialog,350,200,15);
    // 异步说第二句话
    QTimer::singleShot(4000, [=]() {
        fig.setChallengeMovie(":/gif/gif/OscarHablar.gif",80,80,oscarChallengeDialog);
        ico.setSentence ("But the conjungation of Spanish, is also important.",oscarChallengeDialog,350,200,15);
        // 设置按钮，点击按钮时执行结束逻辑
        ico.setButton ("Let's do it!",oscarChallengeDialog,150, 700, 400, 150,[=](){
            fig.setChallengeMovie(":/gif/gif/OscarEsperar.gif",80,80,oscarChallengeDialog);
            // 这里可以添加点击按钮后的结束逻辑，例如关闭对话框
            if (oscarChallengeDialog) {
                oscarChallengeDialog->close();
            }
        });
    });


}

void Parttwo::onOscarDialogClosed()
{
    if (oscarChallengeDialog) {
        oscarChallengeDialog->deleteLater();
        oscarChallengeDialog = nullptr;
    }
}

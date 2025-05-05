#include "scheduleline.h"

ScheduleLine::ScheduleLine()
{

}

void ScheduleLine::setScheduleOne(WordDatabase mdb,QDialog* dialog) {

    int elementCount = m_answerResults1.size();
    QStringList record = (mdb).findExactRecord(":/csv/schedule.csv","id",QString::number(elementCount));
    qDebug()<<QString::number(elementCount);//输出答题了几道
    if (!record.isEmpty()) {
        QString filename = record[1];
        qDebug()<<filename;
        QPixmap pixmap(filename);
        if (!pixmap.isNull()) {
            QPushButton* toolButton = new QPushButton(static_cast<QWidget*>(dialog));
            toolButton->setIcon(QIcon(pixmap));
            toolButton->setIconSize(pixmap.size());
            int x = 600 - pixmap.width(); // 右对齐
            int y = 1000;
            toolButton->move(x, y);
            toolButton->setStyleSheet("QPushButton { border: none; background-color: transparent; }");
            toolButton->show();
        } else {
            qWarning() << "无法加载进度条图片:" << filename;
        }
    }
}

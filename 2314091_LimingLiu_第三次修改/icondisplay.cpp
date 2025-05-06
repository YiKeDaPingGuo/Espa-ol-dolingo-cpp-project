#include "icondisplay.h"

IconDisplay::IconDisplay(){

}


void IconDisplay::setSentence(QString sentence, QWidget *dialog,int x,int y,int size)
{
    QPointer<QWidget> safeDialog(dialog);
    if (!safeDialog || !safeDialog->isVisible()) {  // 新增检查
        qWarning() << "Invalid dialog in setSentence";
        return;
    }


    // 清理之前的句子（如果有）
    if (currentSentenceLabel) {
        currentSentenceLabel->deleteLater();
        currentSentenceLabel = nullptr;
    }
    qDebug()<<sentence;

    // 创建新标签（原实现）

    currentSentenceLabel = new QLabel(dialog);
    QFont font = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
    font.setFamily("OCR A Extended");
    font.setPointSize(size);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QByteArray utf8Data = codec->fromUnicode(sentence);
    QString utf8Sentence = codec->toUnicode(utf8Data);

    currentSentenceLabel->setFont(font);
    currentSentenceLabel->setText(sentence);
    currentSentenceLabel->setWordWrap(true);
    currentSentenceLabel->setFixedWidth(320);
    currentSentenceLabel->adjustSize();
    currentSentenceLabel->setFixedHeight(currentSentenceLabel->height());
    currentSentenceLabel->move(x, y);
    currentSentenceLabel->show();
}

void IconDisplay::setButton(QString sentence, QWidget *dialog, int x, int y, int length, int width, std::function<void()> onClicked)
{
    //qDeleteAll(dialog->findChildren<QPushButton*>());
    QPointer<QWidget> safeDialog(dialog);
    QTimer::singleShot(1000, [=]() {
        if (!safeDialog || safeDialog->isHidden()) { // 更安全的检查方式
            qWarning() << "Dialog invalidated during button creation";
            return;
        }
        if (dialog) {
            QScopedPointer<QPushButton> btnGuard(new QPushButton(sentence, safeDialog));
            QPushButton* btn = btnGuard.get();
            btn->setFont(QFont("OCR A Extended", 15));

            btn->setStyleSheet(
                        "QPushButton {"
                        "   background-color: rgb(166,216,109);"  // 指定背景颜色
                        "   border: none;"                          // 移除边框
                        "   white-space: pre-wrap;"                // 允许自动换行
                        "   text-align: center;"                    // 文字水平居中
                        "   padding: 10px;"                         // 增加内边距
                        "}"
                );

            qDebug() << sentence;
            // 显式保持父对象关系
            btn->setParent(safeDialog);
            btnGuard.take(); // 转移所有权到Qt对象树
            // 计算文本高度
            QFontMetrics metrics(btn->font());
            int textHeight = metrics.boundingRect(QRect(0, 0, length, 0), Qt::TextWordWrap, sentence).height();
            btn->setFixedSize(length, qMax(width, textHeight + 10));

            btn->move(x, y);
            btn->show();
            connect(btn, &QPushButton::clicked, [=]() {
                QTimer::singleShot(500, [=]() {
                    // 删除当前句子和按钮
                    if (currentSentenceLabel) currentSentenceLabel->deleteLater();
                    if (btn) btn->deleteLater();
                    currentSentenceLabel = nullptr;

                    // 执行回调函数（触发下一段对话）
                    if (onClicked) onClicked();
                });
            });
        }
    });
}

QPushButton* IconDisplay::setChoiceButton(QString filename, int x, int y, int length, int width, QWidget *dialog) {
    QPushButton* btn = new QPushButton(dialog);
    btn->setGeometry(QRect(x, y, length, width));

    // 调试：打印按钮属性
    qDebug() << "Button geometry:" << btn->geometry();
    qDebug() << "Parent widget:" << btn->parentWidget();

    // 设置按钮图标（带资源存在性检查）
    qDebug() << "Loading image:" << filename;
    QPixmap pix(filename);
    if (pix.isNull()) {
        qWarning() << "Pixmap is null for" << filename;
        btn->setText("Image Missing");
    } else {
        btn->setIcon(pix.scaled(255, 255, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        btn->setIconSize(QSize(255, 255));
        btn->setText("");
        btn->setStyleSheet("QPushButton { border: none; background-color: transparent; }");
        btn->setWindowOpacity(1.0);
        qDebug() << "Icon set successfully";
    }

    btn->show();
    return btn;
}

void IconDisplay::clear()
{
    if(currentSentenceLabel){
        currentSentenceLabel->deleteLater ();
        currentSentenceLabel = nullptr;
    }
}

#include "explaination.h"
#include "widget.h"

Explaination::Explaination()
{

}

void Explaination::setWords(QStringList sentences, QWidget *dialog, int x, int y, int size)
{
    if (sentences.size() < 2) {
        qDebug() << "Not enough sentences in the list.";
        return;
    }

    // 清理之前的句子（如果有）
    if (currentWordsButton) {
        delete currentWordsButton;
        currentWordsButton = nullptr;
    }
    if (secondWordsLabel) {
        delete secondWordsLabel;
        secondWordsLabel = nullptr;
    }
    clickCount = 0;

    QString sentence = sentences[1];
    qDebug() << "进入啦"<<sentence;

    // 创建新按钮
    currentWordsButton = new QPushButton(sentence, dialog);
    QFont font = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
    font.setFamily("OCR A Extended");
    font.setPointSize(size);
    font.setUnderline(true);
    currentWordsButton->setFont(font);
    currentWordsButton->setFixedWidth(350);
    currentWordsButton->setStyleSheet("QPushButton { border: none; background-color: transparent; }");
    currentWordsButton->move(x, y);
    currentWordsButton->show();

    // 连接按钮点击信号
    connect(currentWordsButton, &QPushButton::clicked, this, [=](){
        clickCount++;
        if (secondWordsLabel) {
            if (clickCount % 2 == 1) {
                secondWordsLabel->show();
            } else {
                secondWordsLabel->hide();
            }
        }
    });

    // 存储第二个句子
    if (sentences.size() > 2) {
        secondWordsLabel = new QLabel(dialog);
        QFont secondFont = font;
        secondFont.setPointSize(size - 3);
        secondWordsLabel->setFont(secondFont);
        secondWordsLabel->setText(sentences[2]);
        secondWordsLabel->setWordWrap(true);
        secondWordsLabel->setFixedWidth(250);
        secondWordsLabel->adjustSize();
        secondWordsLabel->setFixedHeight(secondWordsLabel->height());
        secondWordsLabel->move(x+100, y + currentWordsButton->height());
        secondWordsLabel->hide();
    }
}

void Explaination::clear()
{
    if (currentWordsButton) {
        delete currentWordsButton;
        currentWordsButton = nullptr;
    }
    if (secondWordsLabel) {
        delete secondWordsLabel;
        secondWordsLabel = nullptr;
    }
    clickCount = 0;
}

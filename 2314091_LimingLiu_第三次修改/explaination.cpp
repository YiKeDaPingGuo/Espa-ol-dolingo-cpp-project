#include "explaination.h"
#include "widget.h"

Explaination::Explaination()
{

}

void Explaination::setWords(QStringList sentences, QWidget *dialog, int x, int y, int size,int spanish,int offset)
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

    QString sentence = sentences[spanish];
    qDebug() << "进入啦"<<sentence;

    // 创建新按钮
    currentWordsButton = new QPushButton(sentence, dialog);
    QFont font = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
    font.setFamily("OCR A Extended");
    font.setPointSize(size);
    font.setUnderline(true);
    currentWordsButton->setFont(font);

    currentWordsButton->setFixedWidth(250);
    currentWordsButton->setStyleSheet(        "QPushButton {"
                                              "   border: none;"
                                              "   background-color: transparent;"
                                              "   text-align: left;"          // 添加左对齐
                                              "   padding-left: 5px;"         // 左侧内边距避免文字贴边
                                              "}");
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
        secondWordsLabel->setText(sentences[spanish+offset]);
        secondWordsLabel->setWordWrap(true);
        secondWordsLabel->setFixedWidth(200);
        secondWordsLabel->adjustSize();
        secondWordsLabel->setFixedHeight(secondWordsLabel->height());
        secondWordsLabel->move(x+100, y + currentWordsButton->height());
        secondWordsLabel->hide();
    }
}

void Explaination::setHeader(QString sentence, QWidget *dialog,int x,int y,int size)
{
    // 清理之前的句子（如果有）
    if (currentHeaderLabel) {
        currentHeaderLabel->deleteLater();
        currentHeaderLabel = nullptr;
    }
    qDebug()<<sentence;

    // 创建新标签（原实现）

    currentHeaderLabel = new QLabel(dialog);
    QFont font = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
    font.setFamily("OCR A Extended");
    font.setPointSize(size);

    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QByteArray utf8Data = codec->fromUnicode(sentence);
    QString utf8Sentence = codec->toUnicode(utf8Data);

    currentHeaderLabel->setFont(font);
    currentHeaderLabel->setText(sentence);
    currentHeaderLabel->setWordWrap(true);
    currentHeaderLabel->setFixedWidth(350);
    currentHeaderLabel->adjustSize();
    currentHeaderLabel->setFixedHeight(currentHeaderLabel->height());
    currentHeaderLabel->move(x, y);
    currentHeaderLabel->show();
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
    if(currentHeaderLabel){
        delete currentHeaderLabel;
        currentHeaderLabel = nullptr;
    }
    clickCount = 0;
}

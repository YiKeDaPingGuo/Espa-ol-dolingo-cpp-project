#include "answerchecker.h"

AnswerChecker::AnswerChecker()
{

}

void AnswerChecker::createInputAndCheck(QDialog* dialog, int x, int y, int length, int width, const QString& correctAnswer)
{
    inputBox = new QLineEdit(dialog);

    inputBox->setGeometry(x, y, length, width);
    inputBox->show();
    QString sentence = "Check";

    checkButton = new QPushButton(sentence,dialog);
    checkButton->setFont(QFont("Bauhaus 93", 10));

    checkButton->setStyleSheet(
                "QPushButton {"
                "   background-color: rgb(239,244,128);"  // 指定背景颜色
                "   border: none;"                          // 移除边框
                "   white-space: pre-wrap;"                // 允许自动换行
                "   text-align: center;"                    // 文字水平居中
                "   padding: 10px;"                         // 增加内边距
                "}"
        );

    qDebug()<<sentence;

    // 计算文本高度
    QFontMetrics metrics(checkButton->font());
    int textHeight = metrics.boundingRect(QRect(0, 0, length, 0), Qt::TextWordWrap, sentence).height();
    checkButton->setFixedSize(length, qMax(width, textHeight + 10));

    checkButton->move(x+length+100, y);
    checkButton->show();

    connect(checkButton, &QPushButton::clicked, [=]() {
        QString userAnswer = inputBox->text();
        //userAnswer.toUtf8 ();
        result = (userAnswer == correctAnswer);
        dispCheckAnswer (result,x + length + 100, y);
        qDebug()<<correctAnswer<<" is "<<result;
        isChecked = true;

    });

}

void AnswerChecker::dispCheckAnswer(bool answer, int x, int y)
{
    if (checkButton) {
        // 设置按钮文本
        QString text = answer ? "✔️" : "❌";
        checkButton->setText(text);

        // 设置背景颜色
        QString color = answer ? "221,215,230" : "251,216,187";
        QString style = QString(
            "QPushButton {"
            "   background-color: rgb(%1);"
            "   border: none;"
            "   white-space: pre-wrap;"
            "   text-align: center;"
            "   padding: 10px;"
            "}"
        ).arg(color);

        checkButton->setStyleSheet(style);

        // 可选：根据新文本调整按钮大小（如果需要）
        QFontMetrics metrics(checkButton->font());
        int textWidth = metrics.horizontalAdvance(text);
        int textHeight = metrics.height();
        checkButton->setFixedSize(
            qMax(checkButton->width(), textWidth + 20),  // 保留原始宽度或根据文本扩展
            qMax(checkButton->height(), textHeight + 10)
        );
    }
}

void AnswerChecker::clear()
{
    if (checkButton) {
        delete checkButton;
        checkButton = nullptr;
    }
    if (inputBox) {
        inputBox->hide(); // 隐藏 inputBox
        inputBox->setParent(nullptr); // 从父窗口移除
        delete inputBox;
        inputBox = nullptr;
    }
    isChecked = false;
    result = false;
    icon = QIcon();
    pixmap = QPixmap();
}

bool AnswerChecker::getResult()
{
    return result;
}

bool AnswerChecker::getIsChecked()
{
    return isChecked;
}

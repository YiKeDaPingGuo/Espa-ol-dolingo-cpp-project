#include "sentences.h"

Sentences::Sentences()
{

}

void Sentences::clear()
{
    original.clear();
    filepath.clear();
    english.clear();
    spanish.clear();
    outputresult.clear ();
    Choose = -1;
}


void Sentences::sentenceMaking(int choose,WordDatabase db)
{
    this->Choose = choose;
    qDebug()<<Choose;
    this->Db = db;
    outputresult.clear ();
    switch (Choose) {
        case 3:
        case 9:
            sentenceMakingDative();break;
        case 5:
        case 11:
            sentenceMakingQuestion();break;
        default:
            sentenceMakingSimple();
    }
}

QString Sentences::sentenceConnection(int specie)
{
    QList<QStringList> listall = outputresult;
    QStringList list = listall[specie];
    QString result;
    for (const QString& word : list) {
        if (!word.isEmpty()) {
            if (!result.isEmpty()&&word!="."&&word!="?") {
                result.append(" ");
            }
            result.append(word);
        }
    }
    return result;
}

void Sentences::sentenceMakingSimple()
{
    int choose = Choose % 6;
    original = Db.findExactRecord(":/csv/choice.csv", "id", QString::number(choose + 1));
    original.removeFirst ();
    filepath.clear();
    spanish.clear();
    english.clear();
    for (const QString& str : original) {
        if (!str.isEmpty()) {
            filepath.append(":/csv/" + str + ".csv");
        } else {
            break;
        }
    }

    for (int i = 0; i < filepath.size(); ++i) {
        const QString& str = filepath[i];
        if(str == ":/csv/article.csv"){//未确定阴阳性，先不填
            continue;
        }
        if (str == ":/csv/punct.csv") {
            spanish.append(Db.findExactRecord(str, "id", QString::number(1))[1]);
            english.append(Db.findExactRecord(str, "id", QString::number(1))[2]);
            break;
        }
        int elements = Db.checkRow (str);
        qDebug()<<"elements值为"<<elements;
        int randomNum = QRandomGenerator::global()->bounded(elements)+1;
        qDebug()<<str<<"的随机值是"<<randomNum;
        spanish.append(Db.findExactRecord(str, "id", QString::number(randomNum))[1]);
        english.append(Db.findExactRecord(str, "id", QString::number(randomNum))[2]);
        if(str == ":/csv/neutralwords.csv"){
            int maleNum = QRandomGenerator::global()->bounded(2) * 2 + 1;
            int femaleNum = QRandomGenerator::global()->bounded(2) * 2 + 2;
            QString gender = Db.findExactRecord(str, "id", QString::number(randomNum))[3];
            if(gender == "m"){//若名词为阳性
                spanish.append (Db.findExactRecord(":/csv/article.csv", "id", QString::number(maleNum))[1]);
                english.append (Db.findExactRecord(":/csv/article.csv", "id", QString::number(maleNum))[2]);
            }else{//若名词为阴性
                spanish.append (Db.findExactRecord(":/csv/article.csv", "id", QString::number(femaleNum))[1]);
                english.append (Db.findExactRecord(":/csv/article.csv", "id", QString::number(femaleNum))[2]);
            }
            // 交换 spanish 列表最后两个元素的位置(冠词和名词交换位置)
            if (spanish.size() >= 2) {
                QString temp = spanish[spanish.size() - 1];
                spanish[spanish.size() - 1] = spanish[spanish.size() - 2];
                spanish[spanish.size() - 2] = temp;
            }

            // 交换 english 列表最后两个元素的位置
            if (english.size() >= 2) {
                QString temp = english[english.size() - 1];
                english[english.size() - 1] = english[english.size() - 2];
                english[english.size() - 2] = temp;
            }
        }
    }

    // 首字母大写
    if (!spanish.isEmpty()) {
        QString firstWord = spanish.first();
        if (!firstWord.isEmpty()) {
            firstWord[0] = firstWord[0].toUpper();
            spanish[0] = firstWord;
        }
    }
    if (!english.isEmpty()) {
        QString firstWord = english.first();
        if (!firstWord.isEmpty()) {
            firstWord[0] = firstWord[0].toUpper();
            english[0] = firstWord;
        }
    }

    outputresult.append(spanish);
    outputresult.append(english);
    outputresult.append(spanish);//答案
    //return result;
}

void Sentences::sentenceMakingDative()
{
    int choose = Choose % 6;
    original = Db.findExactRecord(":/csv/choice.csv", "id", QString::number(choose + 1));
    original.removeFirst ();
    filepath.clear();
    spanish.clear();
    english.clear();
    for (const QString& str : original) {
        if (!str.isEmpty()) {
            filepath.append(":/csv/" + str + ".csv");
        } else {
            break;
        }
    }
    //处理与格
    const QString & str0 = filepath[0];
    int elements0 = Db.checkRow (str0);
    qDebug()<<"elements是："<<elements0;
    int randomNum0 = QRandomGenerator::global()->bounded(elements0)+1;
    qDebug()<<"elements值为"<<elements0;
    spanish.append(Db.findExactRecord(str0, "id", QString::number(randomNum0))[1]);
    english.append(Db.findExactRecord(str0, "id", QString::number(randomNum0))[2]);

    // 首字母大写
    if (!spanish.isEmpty()) {
        QString firstWord = spanish.first();
        if (!firstWord.isEmpty()) {
            firstWord[0] = firstWord[0].toUpper();
            spanish[0] = firstWord;
        }
    }
    if (!english.isEmpty()) {
        QString firstWord = english.first();
        if (!firstWord.isEmpty()) {
            firstWord[0] = firstWord[0].toUpper();
            english[0] = firstWord;
        }
    }

    //处理动词
    const QString & str1 = filepath[1];
    if(randomNum0 == 3||randomNum0 == 4){
        spanish.append(Db.findExactRecord(str1, "id", QString::number(2))[1]);
        english.append(Db.findExactRecord(str1, "id", QString::number(2))[2]);
    }else{
        spanish.append(Db.findExactRecord(str1, "id", QString::number(1))[1]);
        english.append(Db.findExactRecord(str1, "id", QString::number(1))[2]);
    }

    //处理冠词和名词
    for (int i = 2; i < filepath.size(); ++i) {
        const QString& str = filepath[i];
        if(str == ":/csv/article.csv"){//未确定阴阳性，先不填
            continue;
        }
        if (str == ":/csv/punct.csv") {
            spanish.append(Db.findExactRecord(str, "id", QString::number(1))[1]);
            english.append(Db.findExactRecord(str, "id", QString::number(1))[2]);
            break;
        }
        int elements = Db.checkRow (str);
        int randomNum = QRandomGenerator::global()->bounded(elements)+1;
        spanish.append(Db.findExactRecord(str, "id", QString::number(randomNum))[1]);
        english.append(Db.findExactRecord(str, "id", QString::number(randomNum))[2]);
        if(str == ":/csv/neutralwords.csv"){
            //只选择定冠词
            int maleNum = 3;
            int femaleNum = 4;
            QString gender = Db.findExactRecord(str, "id", QString::number(randomNum))[3];
            if(gender == "m"){//若名词为阳性
                spanish.append (Db.findExactRecord(":/csv/article.csv", "id", QString::number(maleNum))[1]);
                english.append (Db.findExactRecord(":/csv/article.csv", "id", QString::number(maleNum))[2]);
            }else{//若名词为阴性
                spanish.append (Db.findExactRecord(":/csv/article.csv", "id", QString::number(femaleNum))[1]);
                english.append (Db.findExactRecord(":/csv/article.csv", "id", QString::number(femaleNum))[2]);
            }
            // 交换 spanish 列表最后两个元素的位置(冠词和名词交换位置)
            if (spanish.size() >= 2) {
                QString temp = spanish[spanish.size() - 1];
                spanish[spanish.size() - 1] = spanish[spanish.size() - 2];
                spanish[spanish.size() - 2] = temp;
            }

            // 交换 english 列表最后两个元素的位置
            if (english.size() >= 2) {
                QString temp = english[english.size() - 1];
                english[english.size() - 1] = english[english.size() - 2];
                english[english.size() - 2] = temp;
            }
        }
    }


    //QList<QStringList> result;
    outputresult.append(spanish);//西语注释
    outputresult.append(english);//英语题目
    outputresult.append(spanish);//答案



}

void Sentences::sentenceMakingQuestion()
{
    int choose = Choose % 6;
    original = Db.findExactRecord(":/csv/choice.csv", "id", QString::number(choose + 1));
    original.removeFirst ();
    filepath.clear();
    spanish.clear();
    english.clear();

    for (const QString& str : original) {
        if (!str.isEmpty()) {
            filepath.append(":/csv/" + str + ".csv");
            qDebug()<<"filename:"<<filepath;
        } else {
            break;
        }
    }
    for (int i = 0; i < filepath.size(); ++i) {
        const QString& str = filepath[i];
        if(str == ":/csv/article.csv"){//未确定阴阳性，先不填
            continue;
        }
        if (str == ":/csv/punct.csv") {
            if(i == 0){
               spanish.append(Db.findExactRecord(str, "id", QString::number(2))[1]);
               qDebug()<<"添加"<<Db.findExactRecord(str, "id", QString::number(2))[1];
               english.append(Db.findExactRecord(str, "id", QString::number(2))[2]);
               continue;
            }else{
                spanish.append(Db.findExactRecord(str, "id", QString::number(3))[1]);
                english.append(Db.findExactRecord(str, "id", QString::number(3))[2]);
                qDebug()<<"添加"<<Db.findExactRecord(str, "id", QString::number(3))[1];
                break;
            }
        }
        int elements = Db.checkRow (str) != 0?Db.checkRow (str):1;
        qDebug()<<"elements是："<<elements;
        int randomNum = QRandomGenerator::global()->bounded(elements)+1;
        spanish.append(Db.findExactRecord(str, "id", QString::number(randomNum))[1]);
        english.append(Db.findExactRecord(str, "id", QString::number(randomNum))[2]);
    }

    // 单词首字母大写
    if (spanish.size() >= 2) {
        QString secondWord = spanish[1];
        if (!secondWord.isEmpty()) {
            secondWord[0] = secondWord[0].toUpper();
            spanish[1] = secondWord;
        }
    }

    if (english.size() >= 2) {
        QString secondWord = english[1];
        if (!secondWord.isEmpty()) {
            secondWord[0] = secondWord[0].toUpper();
            english[1] = secondWord;
        }
    }
    outputresult.append(spanish);

    //修正语序
    if (english.size() >= 2) {
        QString temp = english[2];
        english[2] = english[3];
        english[3] = english[4];
        english[4] = temp;
    }
    outputresult.append(english);
    outputresult.append(spanish);


}

QList<QStringList> Sentences::getOutputresult()
{
    return outputresult;
}

QStringList Sentences::getOutputresultAudio()
{
    QString filepath = ":/csv/allaudio.csv";
    QStringList audiolist;
    for(QString str:outputresult[0]){
        qDebug()<<"要播放的内容"<<str;
        if(Db.hasRecord (filepath,"spanish",str)){
            audiolist.append (Db.findExactRecord (filepath,"spanish",str)[1]);
        }
    }
    return audiolist;
}

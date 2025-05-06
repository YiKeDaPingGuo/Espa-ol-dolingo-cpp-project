#include "worddatabase.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

WordDatabase::WordDatabase() {}

#include <QList>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDebug>

QList<QStringList> WordDatabase::loadCSVData(const QString& filePath) {
    QList<QStringList> data;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件:" << filePath;
        return data;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");

    while (!in.atEnd()) {
        QString line = in.readLine();
        data.append(line.split(","));
    }

    file.close();
    return data;
}


void WordDatabase::loadCSVIfNeeded(const QString& filePath) {
    if (m_dataCache.contains(filePath)) return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开文件:" << filePath;
        return;
    }

    QTextStream in(&file);
    in.setCodec("UTF-8");

    // 读取表头
    QString headerLine = in.readLine();
    m_headerCache[filePath] = headerLine.split(",");

    // 读取数据
    QList<QStringList> records;
    while (!in.atEnd()) {
        QString line = in.readLine();
        records.append(line.split(","));
    }

    m_dataCache[filePath] = records;
    file.close();
}

QStringList WordDatabase::findExactRecord(const QString& filePath,
                                             const QString& headerName,
                                             const QString& targetValue) {
    loadCSVIfNeeded(filePath);

    const QStringList& headers = m_headerCache[filePath];
    int columnIndex = headers.indexOf(headerName);
    if (columnIndex == -1) {
        qWarning() << "无效表头:" << headerName;
        return {};
    }

    for (const QStringList& record : m_dataCache[filePath]) {
        if (record.size() > columnIndex &&
            record[columnIndex].compare(targetValue, Qt::CaseInsensitive) == 0) {
            return record;
        }
    }

    qWarning() << "未找到记录:" << targetValue;
    return {};
}

QList<QStringList> WordDatabase::getRandomRecords(const QString& filePath,
                                                int count,
                                                const QStringList& exclude) {
    QList<QStringList> allRecords = m_dataCache[filePath];
    QList<QStringList> candidates;

    for (const QStringList& record : allRecords) {
        if (!exclude.contains(record.join("|"))) {
            candidates.append(record);
        }
    }

    // 随机选择
    QList<QStringList> result;
    for (int i = 0; i < qMin(count, candidates.size()); ++i) {
        int index = QRandomGenerator::global()->bounded(candidates.size());
        result.append(candidates.takeAt(index));
    }

    return result;
}

QList<QStringList> WordDatabase::findWithRandomRecords(const QString& filePath,
                                                       const QString& headerName,
                                                       const QString& targetValue) {
    QStringList mainRecord = findExactRecord(filePath, headerName, targetValue);
    if (mainRecord.isEmpty()) return {};

    QList<QStringList> result;
    result.append(mainRecord);

    // 获取3个随机记录
    QList<QStringList> randoms = getRandomRecords(filePath, 3, {QStringList(mainRecord).join("|")});
    result.append(randoms);

    return result;
}

void WordDatabase::clear()
{
    m_dataCache.clear();
    m_headerCache.clear();
}

QString WordDatabase::getHeaderName(const QString& filePath, int columnIndex)
{
    loadCSVIfNeeded(filePath);
    QStringList headers = m_headerCache.value(filePath);
    if (columnIndex >= 0 && columnIndex < headers.size()) {
        return headers[columnIndex];
    }
    return "";
}


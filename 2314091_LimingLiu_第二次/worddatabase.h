#ifndef WORDDATABASE_H
#define WORDDATABASE_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QStringList>
#include <QRandomGenerator>

class WordDatabase
{
public:
    WordDatabase();

    // 方法1：精确查找
    QStringList findExactRecord(const QString& filePath,
                                  const QString& headerName,
                                  const QString& targetValue);
    // 方法2：选择题查找
    QList<QStringList> findWithRandomRecords(const QString& filePath,
                                        const QString& headerName,
                                        const QString& targetValue);

private:
    QMap<QString, QList<QStringList>> m_dataCache; // 文件路径 -> 数据记录
    QMap<QString, QStringList> m_headerCache;      // 文件路径 -> 表头

    void loadCSVIfNeeded(const QString& filePath);
    QList<QStringList> getRandomRecords(const QString& filePath,
                                      int count,
                                      const QStringList& exclude);
};

#endif // WORDDATABASE_H

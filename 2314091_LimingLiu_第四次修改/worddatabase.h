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

    // 方法1：全部返回

    QList<QStringList> loadCSVData(const QString& filePath) ;

    // 方法2：精确查找
    QStringList findExactRecord(const QString& filePath,
                                  const QString& headerName,
                                  const QString& targetValue);
    // 方法3：选择题查找
    QList<QStringList> findWithRandomRecords(const QString& filePath,
                                        const QString& headerName,
                                        const QString& targetValue);

    // 方法4：清除记录
    void clear();

    // 方法5：获取表头
    QString getHeaderName(const QString& filePath, int columnIndex);

private:
    QMap<QString, QList<QStringList>> m_dataCache; // 文件路径 -> 数据记录
    QMap<QString, QStringList> m_headerCache;      // 文件路径 -> 表头

    void loadCSVIfNeeded(const QString& filePath);
    QList<QStringList> getRandomRecords(const QString& filePath,
                                      int count,
                                      const QStringList& exclude);

};

#endif // WORDDATABASE_H

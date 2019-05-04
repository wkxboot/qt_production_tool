#ifndef DATA_BASE_H
#define DATA_BASE_H

#include <QObject>
#include "QtSql/QSqlDatabase"
#include "QtSql/QSqlQuery"
#include "QSqlError"
#include "QDebug"
#include "QStringList"
#include "qdatetime.h"

class data_base : public QObject
{
    Q_OBJECT
public:
    explicit data_base(QObject *parent = nullptr);
    int insert(QString,QString,QString);
    QMap<QString,QString>  read(int);

signals:

public slots:

private:
    QSqlDatabase *success_record;
    bool valid;
};

#endif // DATA_BASE_H

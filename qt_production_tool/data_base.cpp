#include "data_base.h"

data_base::data_base(QObject *parent) : QObject(parent)
{
    success_record = new QSqlDatabase();
    if (QSqlDatabase::contains("success_record_connection")) {
        *success_record = QSqlDatabase::database("success_record_connection");
    } else {
        *success_record = QSqlDatabase::addDatabase("QSQLITE","success_record_connection");
        success_record->setDatabaseName("success_record.db");
        success_record->setUserName("wkxboot");
        success_record->setPassword("88888888");
    }
    if (!success_record->open("wkxboot","88888888")) {
       qDebug("数据库打开失败！");
       valid = false;
       return;
    } else {
        qDebug("数据库打开成功！");
    }

    QSqlQuery query(*success_record);

    /*判断表是否存在*/
    QStringList strTables = success_record->tables();
    if (!strTables.contains("record")) {

        qDebug("record 表不存在，创建...");
        if (!query.exec("create table record (date varchar(24), sn varchar(24), date_time varchar(24)  primary key )")) {
            qDebug("record创建失败");
            qDebug() << query.lastError();
            valid = false;
            return;
        } else {
            qDebug("record创建成功");
        }
    } else {
        qDebug("record 表已经存在");
    }

    valid = true;
    /* 内部测试
    if (!query.exec("insert into record values('2019-05-05','222222222222','2019-05-05 12:20:20')")) {
      qDebug()<< "insert id =" << "insert into record values('2019-05-05','222222222222','2019-05-05 12:20:20')" << "失败";
    } else {
      qDebug()<< "insert id =" <<"insert into record values('2019-05-05','222222222222','2019-05-05 12:20:20')" << "成功";
    }

    if (!query.exec("insert into record values('2019-05-04','8888888888888','2019-05-04 12:44:33')")) {
      qDebug()<< "insert id =" << "insert into record values('2019-05-04','8888888888888','2019-05-04 12:44:33')" << "失败";
    } else {
      qDebug()<< "insert id =" <<"insert into record values('2019-05-04','8888888888888','2019-05-04 12:44:33')" << "成功";
    }
    */
}


int data_base::insert(QString date,QString sn, QString date_time)
{
    QSqlQuery query(*success_record);
    QString insert = "insert into record values (?,?,?)";
    query.prepare(insert);
    query.addBindValue(date);
    query.addBindValue(sn);
    query.addBindValue(date_time);

    if (!query.exec()) {
        qDebug()<<"插入sn ="<< sn << "date time=" << date_time << "失败";
        return -1;
    } else {
        qDebug()<<"插入sn ="<< sn << "date time=" << date_time << "成功";
    }

    return 0;
}

QMap<QString,QString> data_base::read(int code)
{
    QDateTime time = QDateTime::currentDateTime();

    QMap<QString,QString> read;
    QSqlQuery query(*success_record);
    QString read_info = code == 0 ? "select * from record": "select * from record  where date = ?";
    query.prepare(read_info);

    if (code != 0) {
        query.addBindValue(time.toString("yyyy-MM-dd"));
    }

    if (!query.exec()) {
      qDebug()<< "读取 code =" <<code << "失败";
    } else {
      qDebug()<< "读取 code =" <<code << "成功";

       while (query.next()) {
           read.insert(query.value(1).toString(),query.value(2).toString());
       }
    }
     return read;
}


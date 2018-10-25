#ifndef DB_CONNECTOR_H
#define DB_CONNECTOR_H

#include <QtSql>
#include <QDebug>


class DB_connector
{
public:
    DB_connector() {
        db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setDatabaseName("admin_db");
        db.setUserName("root");
        db.setPassword("93660131");
        isDBCon = db.open();

        if(isDBCon)
            qDebug() << "DB conected!!\n";
        else
            qDebug() << "DB not conected!!\n";
    }

    bool check_login_pass (const QString& login, const QString& pswd) const {
        QString query = "select count(*) as cnt from USERS_DB where LOGIN='" + login + "' and psw='" + pswd + "'";
        qDebug() << query;
        auto ans = db.exec(query);

        ans.first();
        qDebug() << ans.value(1).toInt();

        return ans.value("cnt").toString()=="1" ? true : false;
    }

private:
    QSqlDatabase db;
    bool isDBCon = false;
};

#endif // DB_CONNECTOR_H

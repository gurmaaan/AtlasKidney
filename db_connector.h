#ifndef DB_CONNECTOR_H
#define DB_CONNECTOR_H

#include <QtSql>
#include <QDebug>

class Patient_info
{
public:
    Patient_info() {}
    Patient_info(int idPatient,
                 QString historyNum,
                 int age,
                 int dateOfFallIll,
                 QStringList imagesPaths,
                 QStringList macroFeatures) :
        m_idPatient(idPatient),
        m_historyNum(historyNum),
        m_age(age),
        m_dateOfFallIll(dateOfFallIll),
        m_imagesPaths(imagesPaths),
        m_macroFeatures(macroFeatures) {}

    int idPatient() const;
    QString historyNum() const;
    int age() const;
    int dateOfFallIll() const;
    QStringList imagesPaths() const;
    QStringList macroFeatures() const;

private:
    int m_idPatient;
    QString m_historyNum;
    int m_age;
    int m_dateOfFallIll;
    QStringList m_imagesPaths;
    QStringList m_macroFeatures;
};


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
        QString query = "select  as cnt from USERS_DB where LOGIN='" + login + "' and psw='" + pswd + "'";
        qDebug() << query;
        auto ans = db.exec(query);

        ans.first();

        return ans.value("cnt").toString()=="1" ? true : false;
    }

    QVector<Patient_info> get_all_patients () const {

        QVector<Patient_info> for_ret;

        QString query = "select ID_PATIENT, HISTORY_NUM, AGE, DATE_OF_FALL_ILL from PATIENT";

        auto ans = db.exec(query);

        while(ans.next()){
            int idPatient = ans.value(0).toInt();
            QString historyNum = ans.value(1).toString();
            int age = ans.value(2).toInt();
            int dateOfFallIll = ans.value(3).toInt();
            QStringList imagesPaths;
            QStringList macroFeatures;

            query = "select UNIQUEFILENAME from IMAGES where ID_PATIENT=" + QString::number(idPatient);
            auto tmp_ans = db.exec(query);

            while(tmp_ans.next())
                 imagesPaths.append(tmp_ans.value(0).toString());

            query = "select NAME_MAIN_FEATURE from PATIENT_MACRO_FEATURES p join V_MACRO_MAIN_FEATURES v on p.id_feature = v.ID_MAIN_FEATURE where id_patient=" + QString::number(idPatient);
            tmp_ans = db.exec(query);

            while(tmp_ans.next())
                 macroFeatures.append(tmp_ans.value(0).toString());

            for_ret.append(Patient_info(idPatient, historyNum, age, dateOfFallIll, imagesPaths, macroFeatures));

        }

        return for_ret;
    }

private:
    QSqlDatabase db;
    bool isDBCon = false;
};

#endif // DB_CONNECTOR_H

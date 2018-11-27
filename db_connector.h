#ifndef DB_CONNECTOR_H
#define DB_CONNECTOR_H

#include <QtSql>
#include <QDebug>

class PatientInfo
{
public:
    PatientInfo() {}
    PatientInfo(int idPatient,
                 QString historyNum,
                 int age,
                 int dateOfFallIll,
                 QStringList imagesPaths,
                 QStringList macroFeatures,
                 QChar sex);

    int idPatient() const;
    QString historyNum() const;
    int age() const;
    int dateOfFallIll() const;
    QStringList imagesPaths() const;
    QStringList macroFeatures() const;

    friend QDebug operator<<(QDebug os, const PatientInfo& p);

    //operator QString() const { return "<put your QString here>"; }
private:
    int m_idPatient;
    QString m_historyNum;
    int m_age;
    int m_dateOfFallIll;
    QStringList m_imagesPaths;
    QStringList m_macroFeatures;
    QChar m_sex;
};

QDebug operator<< (QDebug os, const PatientInfo& p);


class DbConnector
{
public:
    DbConnector();
    bool checkLoginPass (const QString& login, const QString& pswd) const;
    QVector<PatientInfo> getAllPatients () const;

private:
    QSqlDatabase db;
    bool isDBCon = false;
};

#endif // DB_CONNECTOR_H

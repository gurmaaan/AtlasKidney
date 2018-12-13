#ifndef DB_CONNECTOR_H
#define DB_CONNECTOR_H

#include <QtSql>
#include <QDebug>

class ImageInfo {
public:
    ImageInfo() {}

    //NOTE: кастыль, потом убрать
    ImageInfo(QString path){ m_path = path;}

    ImageInfo(QString path, QStringList main_feat, QStringList sub_feat) :
        m_path      (path),
        m_main_feat (main_feat),
        m_sub_feat  (sub_feat) {}

    QString     path      () const;
    QStringList main_feat () const;
    QStringList sub_feat  () const;

private:
    QString m_path;
    QStringList m_main_feat;
    QStringList m_sub_feat;
};

class PatientInfo
{
public:
    PatientInfo() {}
    PatientInfo(int idPatient,
                 QString historyNum,
                 int age,
                 int dateOfFallIll,
                 QVector<ImageInfo> images,
                 QChar sex);

    int         idPatient     () const;
    int         age           () const;
    int         dateOfFallIll () const;
    QChar       sex           () const;
    QString     historyNum    () const;
    QVector<ImageInfo> images () const;

    friend QDebug operator<<(QDebug os, const PatientInfo& p);


private:
    int m_idPatient;
    int m_age;
    int m_dateOfFallIll;
    QChar              m_sex;
    QString            m_historyNum;
    QVector<ImageInfo> m_images;
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

#ifndef DB_CONNECTOR_H
#define DB_CONNECTOR_H

#include <QtSql>
#include <QDebug>
#include <QMap>

#include "model/graphicsobject.h"

class Feature
{
public:
    Feature(QVector<QPair<QString, QString>> features, QVector<GraphicsObject> objs) :
        features_(features), objs_(objs)
    {}
    Feature() {}
    QVector<QPair<QString, QString> > features() const;
    QVector<GraphicsObject> objs() const;

private:
    QVector<QPair<QString, QString>> features_;
    QVector<GraphicsObject> objs_;
};

class PatientInfo
{
public:
    PatientInfo() {}
    PatientInfo( QString historyNum,
                 int age,
                 int dateOfFallIll,
                 QMap<QString, QMap<int, Feature>> images,
                 QChar sex);

    int         age           () const;
    int         dateOfFallIll () const;
    QChar       sex           () const;
    QString     historyNum    () const;
//    QMap<QString, QMap<int, Feature>> images () const;

    friend QDebug operator<<(QDebug os, const PatientInfo& p);


    void setImages(const QMap<QString, QMap<int, Feature> > &images);

    bool getIsDownloadedFromDb() const;
    void setIsDownloadedFromDb();

    QMap<QString, QMap<int, Feature> > getImages() const;

private:
    QString            m_historyNum;
    int m_age;
    int m_dateOfFallIll;
    QMap<QString, QMap<int, Feature>> m_images;
    QChar              m_sex;

    bool isDownloadedFromDb; // изменяет состояние после загрузки инфы из БД
};

QDebug operator<< (QDebug os, const PatientInfo& p);

class DbConnector
{
public:
    DbConnector();
    bool checkLoginPass (const QString& login, const QString& pswd) const;
    QMap<int,PatientInfo> getAllPatients () const;
    PatientInfo updatePatientInfoById(int id, PatientInfo &patinf) const;
private:
    QSqlDatabase db;
    bool isDBCon = false;
};

#endif // DB_CONNECTOR_H

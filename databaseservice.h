#ifndef DB_CONNECTOR_H
#define DB_CONNECTOR_H

#include <QtSql>
#include <QDebug>
#include <QMap>

#include "model/graphicsobject.h"
#include "static.h"

class Feature
{
public:
    Feature();
    Feature(QVector<QPair<QString, QString>> features, QVector<GraphicsObject> objs);

    QVector<QPair<QString, QString> > features() const;
    QVector<GraphicsObject> objs() const;

    bool visible() const;
    void setVisible(bool visible);

private:
    QVector<QPair<QString, QString>> features_;
    QVector<GraphicsObject> objs_;
    bool visible_;
};

class PatientInfo
{
public:
    PatientInfo() {}
    PatientInfo( QString historyNum, int age,int dateOfFallIll, QMap<QString, QMap<int, Feature>> images, QChar sex);

    int age() const;
    int dateOfFallIll() const;
    QChar sex() const;
    QString historyNum() const;

    friend QDebug operator<<(QDebug os, const PatientInfo& p);

    void setImages(const QMap<QString, QMap<int, Feature> > &images);

    bool getIsDownloadedFromDb() const;
    void setIsDownloadedFromDb();

    QMap<QString, QMap<int, Feature> > getImages() const;

private:
    QString historyNum_;
    int age_;
    int dateOfFallIll_;
    QMap<QString, QMap<int, Feature>> images_;
    QChar sex_;

    bool isDownloadedFromDb_;
};
QDebug operator<< (QDebug os, const PatientInfo& p);



class DataBaseService
{
public:
    DataBaseService();
    bool checkLoginPass (const QString& login, const QString& pswd) const;
    QMap<int,PatientInfo> getAllPatients () const;
    PatientInfo updatePatientInfoById(int id, PatientInfo &pi) const;
private:
    QSqlDatabase db_;
    bool isDBCon_ = false;
};

#endif // DB_CONNECTOR_H

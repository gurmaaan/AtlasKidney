#include "databaseservice.h"

void PatientInfo::setImages(const QMap<QString, QMap<int, Feature> > &images)
{
    images_ = images;
}

bool PatientInfo::getIsDownloadedFromDb() const
{
    return isDownloadedFromDb_;
}

void PatientInfo::setIsDownloadedFromDb()
{
    isDownloadedFromDb_ = true;
}

QMap<QString, QMap<int, Feature> > PatientInfo::getImages() const
{
    return images_;
}

PatientInfo::PatientInfo(QString historyNum, int age, int dateOfFallIll, QMap<QString, QMap<int, Feature>> images, QChar sex) :
    historyNum_(historyNum), age_(age), dateOfFallIll_(dateOfFallIll), images_(images), sex_(sex), isDownloadedFromDb_(false)
{

}

int PatientInfo::age() const
{
    return age_;
}

int PatientInfo::dateOfFallIll() const
{
    return dateOfFallIll_;
}

QChar PatientInfo::sex() const
{
    return sex_;
}

QString PatientInfo::historyNum() const
{
    return historyNum_;
}

DataBaseService::DataBaseService()
{
    db_ = QSqlDatabase::addDatabase(DB_MySQL);
    db_.setHostName(DB_SERVER);
    db_.setDatabaseName(DB_NAME);
    db_.setUserName(DB_USERNAME);
    db_.setPassword(DB_PASSWORD);
    isDBCon_ = db_.open();

    QString dbStateMsg = isDBCon_ ? DB_OK : DB_NOT_OK;

    qDebug().noquote() << dbStateMsg;
}

bool DataBaseService::checkLoginPass(const QString &login, const QString &pswd) const
{
    QString query = "select count(*) as cnt from users_db where LOGIN='" + login + "' and PSW='" + pswd + "'";
    qDebug() << query;
    auto ans = db_.exec(query);
    ans.first();
    qDebug() << ans.value("cnt").toString();
    return ans.value("cnt").toString()=="1" ? true : false;
}

QMap<int,PatientInfo> DataBaseService::getAllPatients() const
{
    QMap<int,PatientInfo> patientsMap;

    QString query = "select ID_PATIENT, HISTORY_NUM, AGE, DATE_OF_FALL_ILL, SEX from PATIENT where id_organ=9";
    auto ans = db_.exec(query);

    while(ans.next())
    {
        int idPatient = ans.value(0).toInt();
        QString historyNum = ans.value(1).toString();
        int age = ans.value(2).toInt();
        int dateOfFallIll = ans.value(3).toInt();

        QChar sex = ( ans.value(4).toString().length() != 0 ) ? ans.value(4).toString().at(0) : QChar('N');

        query = "select UNIQUEFILENAME from IMAGES where ID_PATIENT=" + QString::number(idPatient);
        auto tmpAns = db_.exec(query);

        QMap<QString, QMap<int, Feature>> images;

        while(tmpAns.next())
            images[tmpAns.value(0).toString()];// = Feature();

        PatientInfo patient(historyNum, age, dateOfFallIll, images, sex);
        patientsMap[idPatient] = patient;
    }

    return patientsMap;
}

PatientInfo DataBaseService::updatePatientInfoById(int id, PatientInfo& pi) const
{
    QString query = "select UNIQUEFILENAME from IMAGES where ID_PATIENT=" + QString::number(id);
    auto tmpAns = db_.exec(query);

    QVector<QString> imgs;

    while(tmpAns.next())
        imgs.append(tmpAns.value(0).toString());

    QMap<QString, QMap<int, Feature>> images;
    for (auto img: imgs)
    {
        query = "select distinct ID_FEATURE\
                from images i join images_micro_features imf on i.ID_IMAGE=imf.ID_IMAGE \
                where UNIQUEFILENAME=\"" + img + "\"";
        auto tmpAns = db_.exec(query);

        QVector<int> featureIDs;
        while(tmpAns.next())
            featureIDs.append(tmpAns.value(0).toInt());

        QMap<int, Feature> allFeatures;
        for (auto feat: featureIDs)
        {
            query = "select \
                    NAME_MAIN_FEATURE, \
                    NAME_SUB_FEATURE \
            from  \
                images_micro_features       imf  \
                join v_micro_features            vmf  on vmf.ID_FEATURE        = imf.ID_FEATURE \
                join v_micro_main_features       vmmf on vmmf.ID_MAIN_FEATURE  = vmf.ID_MAIN_FEATURE \
                join v_micro_sub_features        vmsf on vmsf.ID_SUB_FEATURE   = vmf.ID_SUB_FEATURE where imf.ID_IMAGE=" + QString::number(feat);
            auto tmpAns = db_.exec(query);

            QVector<QPair<QString, QString>> features;
            while(tmpAns.next())
                features.append(QPair<QString, QString>(tmpAns.value(0).toString(), tmpAns.value(1).toString()));

            query = "select descr_type, p1,p2,p3,p4 \
                    from img_descr_info \
                    where id_feature=" + QString::number(feat) + " and img_name=\"" + img + "\"";
            tmpAns = db_.exec(query);

            QVector<GraphicsObject> objs;
            while(tmpAns.next())
            {
                QString signStr = tmpAns.value(0).toString();

                qreal p1 = tmpAns.value(1).toInt();
                qreal p2 = tmpAns.value(2).toInt();
                qreal p3 = tmpAns.value(3).toInt();
                qreal p4 = tmpAns.value(4).toInt();

                GraphicsObject go;
                go.setStartPoint(QPointF(p1,p2));
                go.setEndPoint(QPointF(p3,p4));
                //FIXME func setType should clear all other signs at the begining and end
                go.setType("\"" + signStr + "\"");

                objs.append(go);
            }
            Feature f (features, objs);
            allFeatures[feat] = f;
        }
        images[img] = allFeatures;
    }
    pi.setImages(images);
    pi.setIsDownloadedFromDb();
    return pi;
}



Feature::Feature(QVector<QPair<QString, QString> > features, QVector<GraphicsObject> objs) :
    features_(features), objs_(objs)
{}

Feature::Feature() {}

QVector<QPair<QString, QString> > Feature::features() const
{
    return features_;
}

QVector<GraphicsObject> Feature::objs() const
{
    return objs_;
}

bool Feature::visible() const
{
    return visible_;
}

void Feature::setVisible(bool visible)
{
    visible_ = visible;
}

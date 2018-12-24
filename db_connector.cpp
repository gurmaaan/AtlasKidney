#include "db_connector.h"

int                PatientInfo::age()           const { return m_age;           }
int                PatientInfo::dateOfFallIll() const { return m_dateOfFallIll; }
QChar              PatientInfo::sex()           const { return m_sex;           }
QString            PatientInfo::historyNum()    const { return m_historyNum;    }

void PatientInfo::setImages(const QMap<QString, QMap<int, Feature> > &images)
{
    m_images = images;
}

bool PatientInfo::getIsDownloadedFromDb() const
{
    return isDownloadedFromDb;
}

void PatientInfo::setIsDownloadedFromDb()
{
    isDownloadedFromDb = true;
}

QMap<QString, QMap<int, Feature> > PatientInfo::getImages() const
{
    return m_images;
}

PatientInfo::PatientInfo(QString historyNum, int age, int dateOfFallIll, QMap<QString, QMap<int, Feature>> images, QChar sex) :
    m_historyNum(historyNum),
    m_age(age),
    m_dateOfFallIll(dateOfFallIll),
    m_images(images),
    m_sex(sex),
    isDownloadedFromDb(false)
{}

DbConnector::DbConnector() {
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("admin_db");
    db.setUserName("root");
    db.setPassword("toor");
    isDBCon = db.open();

    if(isDBCon)
        qDebug() << "DB conected!!\n";
    else
        qDebug() << "DB not conected!!\n";

}

bool DbConnector::checkLoginPass(const QString &login, const QString &pswd) const {
    QString query = "select count(*) as cnt from USERS_DB where LOGIN='" + login + "' and psw='" + pswd + "'";

    auto ans = db.exec(query);

    ans.first();

    return ans.value("cnt").toString()=="1" ? true : false;
}

QMap<int,PatientInfo> DbConnector::getAllPatients() const {

    QMap<int,PatientInfo> for_ret;

    QString query = "select ID_PATIENT, HISTORY_NUM, AGE, DATE_OF_FALL_ILL, SEX from PATIENT where id_organ=9";

    auto ans = db.exec(query);

    while(ans.next()){

        int idPatient = ans.value(0).toInt();
        QString historyNum = ans.value(1).toString();
        int age = ans.value(2).toInt();
        int dateOfFallIll = ans.value(3).toInt();

        QChar sex = (ans.value(4).toString().size() != 0) ? ans.value(4).toString()[0] : QChar('N');

        query = "select UNIQUEFILENAME from IMAGES where ID_PATIENT=" + QString::number(idPatient);
        auto tmp_ans = db.exec(query);

//        QVector<QString> imgs;
        QMap<QString, QMap<int, Feature>> images;

        while(tmp_ans.next())
            images[tmp_ans.value(0).toString()];// = Feature();

        PatientInfo patient(historyNum, age, dateOfFallIll, images, sex);

        for_ret[idPatient] = patient;

    }


    return for_ret;
}

PatientInfo DbConnector::updatePatientInfoById(int id, PatientInfo& patinf) const {

    QString query = "select UNIQUEFILENAME from IMAGES where ID_PATIENT=" + QString::number(id);
    auto tmp_ans = db.exec(query);

    QVector<QString> imgs;

    while(tmp_ans.next())
        imgs.append(tmp_ans.value(0).toString());

    QMap<QString, QMap<int, Feature>> images;

    for (auto img: imgs){



        query = "select distinct ID_FEATURE\
                from images i join images_micro_features imf on i.ID_IMAGE=imf.ID_IMAGE \
                where UNIQUEFILENAME=\"" + img + "\"";

        auto tmp_ans = db.exec(query);

        QVector<int> features_id;
        while(tmp_ans.next())
            features_id.append(tmp_ans.value(0).toInt());

        QMap<int, Feature> one_feat;

        for (auto feat: features_id){
            query = "select \
                    NAME_MAIN_FEATURE, \
                    NAME_SUB_FEATURE \
            from  \
                images_micro_features       imf  \
                join v_micro_features            vmf  on vmf.ID_FEATURE        = imf.ID_FEATURE \
                join v_micro_main_features       vmmf on vmmf.ID_MAIN_FEATURE  = vmf.ID_MAIN_FEATURE \
                join v_micro_sub_features        vmsf on vmsf.ID_SUB_FEATURE   = vmf.ID_SUB_FEATURE where imf.ID_IMAGE=" + QString::number(feat);

            auto tmp_ans = db.exec(query);

            QVector<QPair<QString, QString>> features;

            while(tmp_ans.next())
                features.append(QPair<QString, QString>(tmp_ans.value(0).toString(), tmp_ans.value(1).toString()));

            query = "select descr_type, p1,p2,p3,p4 \
                    from img_descr_info \
                    where id_feature=" + QString::number(feat) + " and img_name=\"" + img + "\"";

            tmp_ans = db.exec(query);

            QVector<GraphicsObject> objs;

            while(tmp_ans.next()){
                QString sign_str = tmp_ans.value(0).toString();
//                Sign sign;
//                if(sign_str == "Arrow")        sign = Sign::Arrow;
//                else if(sign_str == "Square")  sign = Sign::Square;
//                else if(sign_str == "Ellipse") sign = Sign::Ellipse;

                float p1 = tmp_ans.value(1).toInt();
                float p2 = tmp_ans.value(2).toInt();
                float p3 = tmp_ans.value(3).toInt();
                float p4 = tmp_ans.value(4).toInt();

                GraphicsObject go;

                go.setStartPoint(QPointF(p1,p2));
                go.setEndPoint(QPointF(p3,p4));

                //FIXME func setType should clear all other signs at the begining and end
                go.setType("\"" + sign_str + "\"");

                objs.append(go);

            }
            Feature f (features, objs);

            one_feat[feat] = f;
        }

        images[img] = one_feat;

    }

    patinf.setImages(images);

    patinf.setIsDownloadedFromDb();

    return patinf;
}



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

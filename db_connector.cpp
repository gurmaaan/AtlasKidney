#include "db_connector.h"

int                PatientInfo::idPatient()     const { return m_idPatient;     }
int                PatientInfo::age()           const { return m_age;           }
int                PatientInfo::dateOfFallIll() const { return m_dateOfFallIll; }
QChar              PatientInfo::sex()           const { return m_sex;           }
QString            PatientInfo::historyNum()    const { return m_historyNum;    }
QVector<ImageInfo> PatientInfo::images()        const { return m_images;        }

QString     ImageInfo::path()            const { return m_path;          }
QStringList ImageInfo::main_feat()       const { return m_main_feat;     }
QStringList ImageInfo::sub_feat()        const { return m_sub_feat;      }

PatientInfo::PatientInfo(int idPatient, QString historyNum, int age, int dateOfFallIll, QVector<ImageInfo> images, QChar sex) :
    m_idPatient(idPatient),
    m_historyNum(historyNum),
    m_age(age),
    m_dateOfFallIll(dateOfFallIll),
    m_images(images),
    m_sex(sex)
{}



QDebug operator<<(QDebug os, const PatientInfo &p) {
    os << "idPatient: " << p.m_idPatient <<
          "\n\thistoryNum: " << p.m_historyNum <<
    "\n\tage: " << p.m_age <<
    "\n\tdateOfFallIll: " << p.m_dateOfFallIll <<
    "\n\tsex: " << QString(p.m_sex) <<
    "\n\timagesPaths:\n";
//    for(auto i: p.m_imagesPaths)
//        os << "\t\t" << i << "\n";
//    os << "\n\tmacroFeatures:\n";
//    for(auto i: p.m_macroFeatures)
//        os << "\t\t" << i << "\n";
    return os;
}

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

    auto vec = getAllPatients();

    qDebug() << vec[0];

}

bool DbConnector::checkLoginPass(const QString &login, const QString &pswd) const {
    QString query = "select count(*) as cnt from USERS_DB where LOGIN='" + login + "' and psw='" + pswd + "'";
    qDebug() << query;
    auto ans = db.exec(query);

    ans.first();

    return ans.value("cnt").toString()=="1" ? true : false;
}

QVector<PatientInfo> DbConnector::getAllPatients() const {

    QVector<PatientInfo> for_ret;

    QString query = "select ID_PATIENT, HISTORY_NUM, AGE, DATE_OF_FALL_ILL, SEX from PATIENT where id_organ=9";

    auto ans = db.exec(query);

    while(ans.next()){
        int idPatient = ans.value(0).toInt();
        QString historyNum = ans.value(1).toString();
        int age = ans.value(2).toInt();
        int dateOfFallIll = ans.value(3).toInt();
        QVector<ImageInfo> images;
        QChar sex = (ans.value(4).toString().size() != 0) ? ans.value(4).toString()[0] : QChar('N');

        query = "select UNIQUEFILENAME from IMAGES where ID_PATIENT=" + QString::number(idPatient);
        auto tmp_ans = db.exec(query);

        while(tmp_ans.next())
            images.append(tmp_ans.value(0).toString());

        //TODO:: Переделать под микропризнаки с макропризнаков!!!!!!!!!!
        query = "select     NAME_MAIN_FEATURE, \
                            NAME_SUB_FEATURE \
                from images i \
                    join images_micro_features       imf  on imf.ID_IMAGE          = i.ID_IMAGE \
                    join v_micro_features            vmf  on vmf.ID_FEATURE        = imf.ID_FEATURE \
                    join v_micro_main_features       vmmf on vmmf.ID_MAIN_FEATURE  = vmf.ID_MAIN_FEATURE \
                    join v_micro_sub_features        vmsf on vmsf.ID_SUB_FEATURE   = vmf.ID_SUB_FEATURE \
                where i.ID_PATIENT=" + QString::number(idPatient);
//        tmp_ans = db.exec(query);

//        while(tmp_ans.next())
//            qDebug() << "11 " << tmp_ans.value(0).toString() << ": " << tmp_ans.value(1).toString();

//        exit(1);
//            macroFeatures.append(tmp_ans.value(0).toString());

        for_ret.append(PatientInfo(idPatient, historyNum, age, dateOfFallIll, images, sex));

    }

    return for_ret;
}


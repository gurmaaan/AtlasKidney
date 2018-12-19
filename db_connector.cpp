#include "db_connector.h"

PatientInfo::PatientInfo(int idPatient, QString historyNum, int age, int dateOfFallIll, QStringList imagesPaths, QStringList macroFeatures, QChar sex) :
    m_idPatient(idPatient),
    m_historyNum(historyNum),
    m_age(age),
    m_dateOfFallIll(dateOfFallIll),
    m_imagesPaths(imagesPaths),
    m_macroFeatures(macroFeatures),
    m_sex(sex)
{}

int PatientInfo::idPatient() const
{
    return m_idPatient;
}

QString PatientInfo::historyNum() const
{
    return m_historyNum;
}

int PatientInfo::age() const
{
    return m_age;
}

int PatientInfo::dateOfFallIll() const
{
    return m_dateOfFallIll;
}

QStringList PatientInfo::imagesPaths() const
{
    return m_imagesPaths;
}

QStringList PatientInfo::macroFeatures() const
{
    return m_macroFeatures;
}

QDebug operator<<(QDebug os, const PatientInfo &p) {
    os << "idPatient: " << p.m_idPatient <<
    "\n\thistoryNum: " << p.m_historyNum <<
    "\n\tage: " << p.m_age <<
    "\n\tdateOfFallIll: " << p.m_dateOfFallIll <<
    "\n\tsex: " << QString(p.m_sex) <<
    "\n\timagesPaths:\n";
    for(auto i: p.m_imagesPaths)
        os << "\t\t" << i << "\n";
    os << "\n\tmacroFeatures:\n";
    for(auto i: p.m_macroFeatures)
        os << "\t\t" << i << "\n";
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
//    for (auto i: vec)
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
        QStringList imagesPaths;
        QStringList macroFeatures;
        QChar sex = (ans.value(4).toString().size() != 0) ? ans.value(4).toString()[0] : QChar('N');

        query = "select UNIQUEFILENAME from IMAGES where ID_PATIENT=" + QString::number(idPatient);
        auto tmp_ans = db.exec(query);

        while(tmp_ans.next())
            imagesPaths.append(tmp_ans.value(0).toString());

        query = "select NAME_MAIN_FEATURE from PATIENT_MACRO_FEATURES p join V_MACRO_MAIN_FEATURES v on p.id_feature = v.ID_MAIN_FEATURE where id_patient=" + QString::number(idPatient);
        tmp_ans = db.exec(query);

        while(tmp_ans.next())
            macroFeatures.append(tmp_ans.value(0).toString());

        for_ret.append(PatientInfo(idPatient, historyNum, age, dateOfFallIll, imagesPaths, macroFeatures, sex));

    }

    return for_ret;
}

#include "db_connector.h"

int Patient_info::idPatient() const
{
    return m_idPatient;
}

QString Patient_info::historyNum() const
{
    return m_historyNum;
}

int Patient_info::age() const
{
    return m_age;
}

int Patient_info::dateOfFallIll() const
{
    return m_dateOfFallIll;
}

QStringList Patient_info::imagesPaths() const
{
    return m_imagesPaths;
}

QStringList Patient_info::macroFeatures() const
{
    return m_macroFeatures;
}

QDebug operator<<(QDebug os, const Patient_info &p) {
    os << "idPatient: " << p.m_idPatient <<
    "\n\thistoryNum: " << p.m_historyNum <<
    "\n\tage: " << p.m_age <<
    "\n\tdateOfFallIll: " << p.m_dateOfFallIll <<
    "\n\timagesPaths:\n";
    for(auto i: p.m_imagesPaths)
        os << "\t\t" << i << "\n";
    os << "\n\tmacroFeatures:\n";
    for(auto i: p.m_macroFeatures)
        os << "\t\t" << i << "\n";
    return os;
}

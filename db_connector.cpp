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

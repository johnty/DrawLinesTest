#include "qmapperdbmodel.h"

QMapperDbModel::QMapperDbModel()
{

}

void QMapperDbModel::LoadFromTest()
{
    //make some devices with sigs
    QString dev1name = "TestDev01";
    mapperDevNames.append(dev1name);

    for (int i=0; i<3; ++i)
    {
        QString signame = "signame" + QString::number(i+1);
        QStandardItem* newSig = new QStandardItem(signame);

        newSig->insertRow(0, new QStandardItem(dev1name));
        newSig->insertRow(1, new QStandardItem("props"));


        mapperSignals.append(newSig);
    }


    //make some maps

}

const QString QMapperDbModel::getSigName(int idx)
{
    return mapperSignals.at(idx)->text();
}

const QString QMapperDbModel::getSigDevName(int idx)
{
    return mapperSignals.at(idx)->child(0)->text();
}

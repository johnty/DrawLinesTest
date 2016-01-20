#include "qmapperdbmodel.h"

QMapperDbModel::QMapperDbModel()
{

}

void QMapperDbModel::LoadFromTest(int testDB)
{
    //make some devices with sigs
    //some hard coded test situations

    //we use same devices and signals for each test sample:

    QString dev1name = "TestDev01";
    mapperDevNames.append(dev1name);

    for (int i=0; i<3; ++i)
    {
        QString signame = "signame" + QString::number(i+1);
        QStandardItem* newSig = new QStandardItem(signame);

        newSig->insertRow(0, new QStandardItem(dev1name));
        newSig->insertRow(1, new QStandardItem("output"));

        mapperSignals.append(newSig);
        qDebug() << "signal " <<mapperSignals.size()-1<< " is output";
    }

    dev1name = "Synth01";
    mapperDevNames.append(dev1name);
    for (int i=0; i<5; ++i)
    {
        QString signame = "signame" + QString::number(i+1);
        QStandardItem* newSig = new QStandardItem(signame);

        newSig->insertRow(0, new QStandardItem(dev1name));
        newSig->insertRow(1, new QStandardItem("input"));

        mapperSignals.append(newSig);
        qDebug() << "signal " <<mapperSignals.size()-1<< " is input";
    }

    /*
signal  0  is output
signal  1  is output
signal  2  is output
signal  3  is input
signal  4  is input
signal  5  is input
signal  6  is input
signal  7  is input
     */

    //make some maps
    if (testDB == 0)
    {
        mapperMapsSrc.append(0);
        mapperMapsDst.append(4);

        mapperMapsSrc.append(0);
        mapperMapsDst.append(5);

        mapperMapsSrc.append(2);
        mapperMapsDst.append(7);
    }
    else if (testDB == 1)
    {
        mapperMapsSrc.append(1);
        mapperMapsDst.append(4);

        mapperMapsSrc.append(1);
        mapperMapsDst.append(5);

        mapperMapsSrc.append(2);
        mapperMapsDst.append(7);
    }

}

const QString QMapperDbModel::getSigName(int idx)
{
    return mapperSignals.at(idx)->text();
}

const QString QMapperDbModel::getSigDevName(int idx)
{
    return mapperSignals.at(idx)->child(0)->text();
}

const bool QMapperDbModel::isOutputSig(int idx)
{
    return (mapperSignals.at(idx)->child(1)->text() == "output");
}

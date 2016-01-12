#include "qmapperdbmodel.h"

QMapperDbModel::QMapperDbModel()
{

}

void QMapperDbModel::LoadFromTest()
{
    //make some devices with sigs
    QStrin dev1name = "TestDev01";
    mapperDevNames.append("Test 01");

    QStandardItem newSig;
    newSig.setText("signame 01");
    newSig.insertRow(0, new QStandardItem(dev1name));
    newSig.insertRow(1, new QStandardItem("props"));
    mapperSignals.append(newSig);


    //make some maps


}

#ifndef QMAPPERDBMODEL_H
#define QMAPPERDBMODEL_H

#include <QStandardItemModel>
#include <QObject>

// represents the entire state of the mapper network's devices, signals, and maps for drawing
// with the QT framework GUI objects
// Interfaces with:
//    git loader
//    QSql (is this still necessary?)
//    Mapper DB
//    dummy test


class QMapperDbModel
{
public:
    QMapperDbModel();

    //create a bunch of test devices/signals/maps
    void LoadFromTest();

    int getNumSigs() {
        return mapperSignals.size();
    }

    const QStandardItem* getSigItem(int index) {return mapperSignals.at(index);}
    const QString getSigName(int idx);
    const QString getSigDevName(int idx);
    const bool isOutputSig(int idx);

private:

    //TODO: more organized data structures
    QVector<QString> mapperDevNames;
    QVector<QStandardItem*> mapperSignals;
    QVector<QStandardItem*> mapperMaps;
};

#endif // QMAPPERDBMODEL_H

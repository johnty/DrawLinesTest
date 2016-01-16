#include "qmapperdbscene.h"

QMapperDbScene::QMapperDbScene(QObject *parent) : QGraphicsScene(parent)
{
    dbModel = NULL;

    tempPath.moveTo(0,32);
    tempPath.lineTo(100, 100);

    tempPathItem.setPath(tempPath);
    tempPathItem.setPen(QPen(Qt::red, 2));
    tempPathItem.setBrush(QBrush(Qt::red));
    addItem(&tempPathItem);

}

void QMapperDbScene::mouseDropped(QPointF pos)
{
    qDebug() << "dbScene dropped @ " << pos;
    tempPathItem.setVisible(false);
}

void QMapperDbScene::mouseDragged(QPointF pos)
{
    tempPathItem.setVisible(true);
    qDebug() <<"dbScene dragged @ " << pos;
    mapPt = pos;
    mapPt.setY( mapPt.y()+32);
    updateMaps();
    //updateScene();


}

void QMapperDbScene::updateMaps()
{

    tempPath = QPainterPath();
    tempPath.moveTo(0,32);
    //path.cubicTo(0+5, 0, 150, 200, 200, 200);
    tempPath.lineTo(mapPt);
    tempPathItem.setPath(tempPath);

}

void QMapperDbScene::updateScene()
{
    if (dbModel != NULL)
    {

        removeItem(&tempPathItem);
        clear();
        sigs.clear();

        int inputOffsetY = 0;
        int outputOffsetY = 0;
        for (int i=0; i<dbModel->getNumSigs(); ++i)
        {
            QString devname = dbModel->getSigDevName(i);
            QString signame = dbModel->getSigName(i);
            CustomRect * sigrect;
            int offsetY;
            int offsetX;
            if (dbModel->isOutputSig(i))\
            {
                offsetX = 0;
                outputOffsetY += 35;
                offsetY = outputOffsetY;
            }
            else
            {
                offsetX = 200;
                inputOffsetY += 35;
                offsetY = inputOffsetY;
            }
            sigrect = new CustomRect(offsetX, offsetY, devname, signame);
            QObject::connect(sigrect, SIGNAL(mouseDragSig(QPointF)), this, SLOT(mouseDragged(QPointF)));
            QObject::connect(sigrect, SIGNAL(mouseDropSig(QPointF)), this, SLOT(mouseDropped(QPointF)));
            sigs.push_back(sigrect);
            addItem(sigrect);
        }
        addItem(&tempPathItem);

        //updateMaps();

    }
}

void QMapperDbScene::setMapperDbModel(QMapperDbModel* model)
{
    dbModel = model;
}

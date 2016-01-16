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

//void QMapperDbScene::mouseMoveEvent(QGraphicsSceneMouseEvent *e)
//{
//    qDebug() <<"dbscene mmove " << e->pos();
//}

void QMapperDbScene::mouseDropped(QPointF pos)
{
    qDebug() << "dbScene dropped @ " << pos;
    tempPathItem.setVisible(false);
}

void QMapperDbScene::mouseDragged(QPointF pos)
{
    tempPathItem.setVisible(true);
    qDebug() <<"dbScene dragged @ " << pos;
    mapPtDst = pos;
    mapPtDst.setY( mapPtDst.y());
    updateMaps();
    //updateScene();
}

void QMapperDbScene::mouseDropped(QPointF src, QPointF dst)
{
    qDebug() <<"dbScene DROP from " << src <<" to " << dst;


}

void QMapperDbScene::mouseDragged(QPointF src, QPointF dst)
{
    //soooo ... many... hacks....

    tempPathItem.setVisible(true);
    qDebug() <<"dbScene DRAG from " << src <<" to " << dst;
    mapPtSrc = src;

    //TODO: find this stupid offset!
    mapPtSrc.setY(mapPtSrc.y());
    mapPtDst = dst;
    mapPtDst.setY( mapPtDst.y() +mapPtSrc.y());
    mapPtDst.setX( mapPtDst.x() +mapPtSrc.x());
    updateMaps();
    //updateScene();
}

void QMapperDbScene::updateMaps()
{
    tempPath = QPainterPath();
    tempPath.moveTo(mapPtSrc);
    //path.cubicTo(0+5, 0, 150, 200, 200, 200);
    tempPath.lineTo(mapPtDst);
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
                outputOffsetY += MAPPER_SCENE_ITEM_H + MAPPER_SCENE_SPACER;
                offsetY = outputOffsetY;
            }
            else
            {
                offsetX = MAPPER_SCENE_ITEM_W*8;
                inputOffsetY += MAPPER_SCENE_ITEM_H + MAPPER_SCENE_SPACER;
                offsetY = inputOffsetY;
            }
            sigrect = new CustomRect(offsetX, offsetY, devname, signame);
            //QObject::connect(sigrect, SIGNAL(mouseDragSig(QPointF)), this, SLOT(mouseDragged(QPointF)));
            QObject::connect(sigrect, SIGNAL(mouseDragSig(QPointF, QPointF)), this, SLOT(mouseDragged(QPointF, QPointF)));
            //QObject::connect(sigrect, SIGNAL(mouseDropSig(QPointF)), this, SLOT(mouseDropped(QPointF)));
            QObject::connect(sigrect, SIGNAL(mouseDropSig(QPointF, QPointF)), this, SLOT(mouseDropped(QPointF, QPointF)));
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

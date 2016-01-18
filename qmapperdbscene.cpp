#include "qmapperdbscene.h"

QMapperDbScene::QMapperDbScene(QObject *parent) : QGraphicsScene(parent)
{
    dbModel = NULL;

    tempPath.moveTo(0,32);
    tempPath.lineTo(100, 100);

    tempPathItem.setPath(tempPath);
    tempPathItem.setPen(QPen(Qt::red, 2));
    //tempPathItem.setBrush(QBrush(Qt::red));
    tempPathItem.setVisible(false);
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
    for (int i=0; i<sigs.size(); ++i)
    {
        sigs.at(i)->setHovered(false);
    }
    tempPathItem.setVisible(false);


}

void QMapperDbScene::mouseDragged(QPointF src, QPointF dst)
{
    //soooo ... many... hacks....

    tempPathItem.setVisible(true);
    qDebug() <<"dbScene DRAG from " << src <<" to " << dst;

    //first, get the location of where we should be drawing
    mapPtSrc = src;

    mapPtSrc.setX(mapPtSrc.x()+MAPPER_SCENE_ITEM_W);
    mapPtSrc.setY(mapPtSrc.y()+MAPPER_SCENE_ITEM_H/2);

    mapPtSrc.setY(mapPtSrc.y());
    mapPtDst = dst;
    mapPtDst.setY( mapPtDst.y() +src.y());
    mapPtDst.setX( mapPtDst.x() +src.x());

    //find index of source object in list

    int srcIdx = getIndexOfSigNear(mapPtSrc, 10);

    //then, check if we've moved over an item...

    int hoverIdx = getIndexOfSigNear(mapPtDst, 10);

    //and if so, set it to render with selected graphic
    for (int i=0; i<sigs.size(); ++i)
    {
        if ((i == hoverIdx) && (srcIdx != hoverIdx))
        {
            sigs.at(i)->setHovered(true);
        }
        else
        {   //unfortunately we have to also set everything else to
            // non-hovered, unless we have some sort of exit
            // mechanism, would would require a similar kind of search
            // or better manangement of the child items.
            // TODO: this potentially motivates the child objects (CustomRect)
            // to hold onto an external index? this way we can simply emit
            // the src and dst object indicies...
            sigs.at(i)->setHovered(false);
        }
    }


    //make end of temporary arrow at the centre of receiving object
    // if available, and make sure its not itself
    if ( (hoverIdx != -1) && (hoverIdx != srcIdx))
    {
        mapPtDst.setX(sigs.at(hoverIdx)->boundingRectAbs().left());
        mapPtDst.setY(sigs.at(hoverIdx)->boundingRectAbs().top() + MAPPER_SCENE_ITEM_H/2);
    }

    //draw map path
    updateMaps();

    //updateScene();
}

void QMapperDbScene::updateMaps()
{
    //temporary line (while dragging)
    tempPath = QPainterPath();
    tempPath.moveTo(mapPtSrc);
    tempPath.cubicTo(mapPtSrc.x()+MAPPER_SCENE_CURVE, mapPtSrc.y(), mapPtDst.x()-MAPPER_SCENE_CURVE, mapPtDst.y(), mapPtDst.x(), mapPtDst.y());
    //tempPath.lineTo(mapPtDst);
    tempPathItem.setPath(tempPath);


    //maps list

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
                offsetX = MAPPER_SCENE_ITEM_W*2 + MAPPER_SCENE_SPACER*2;
                inputOffsetY += MAPPER_SCENE_ITEM_H + MAPPER_SCENE_SPACER;
                offsetY = inputOffsetY;
            }
            sigrect = new CustomRect(offsetX, offsetY, devname, signame);
            //QObject::connect(sigrect, SIGNAL(mouseDragSig(QPointF)), this, SLOT(mouseDragged(QPointF)));
            QObject::connect(sigrect, SIGNAL(mouseDragSig(QPointF, QPointF)), this, SLOT(mouseDragged(QPointF, QPointF)));
            //QObject::connect(sigrect, SIGNAL(mouseDropSig(QPointF)), this, SLOT(mouseDropped(QPointF)));
            QObject::connect(sigrect, SIGNAL(mouseDropSig(QPointF, QPointF)), this, SLOT(mouseDropped(QPointF, QPointF)));
            QObject::connect(sigrect, SIGNAL(mousePressSig()), this, SLOT(mousePressed()));
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

int QMapperDbScene::getIndexOfSigNear(QPointF pos, float len)
{
    int foundIdx = -1;
    for (int i=0; i<sigs.size(); ++i)
    {
        QRectF dragHitRect(pos.x()-len/2, pos.y()-len/2, len, len);

        if (dragHitRect.intersects(sigs.at(i)->boundingRectAbs()))
        {
            qDebug() << "hit sig # " << i;
            foundIdx = i;
            break; //assume no overlapping boxes...
        }

    }
    return foundIdx;
}

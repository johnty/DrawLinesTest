#include "qmapperdbscene.h"

QMapperDbScene::QMapperDbScene(QObject *parent) : QGraphicsScene(parent), activeLayer(this)
{
    dbModel = NULL;
    addItem(&tempPathItem);
    tempPathItem.setPen(QPen(Qt::red, 2));
    //setForegroundBrush(QColor(255, 255, 255, 120));
    //addItem(activeLayer.getLayerItems());
    tempPathItem.setVisible(false);
}

QMapperDbScene::~QMapperDbScene()
{
    removeItem(activeLayer.getLayerItems());
}

void QMapperDbScene::mouseDoubleClicked()
{
    qDebug() <<"dbl";
    updateScene();
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
{ //NOT USED
    tempPathItem.setVisible(true);
    qDebug() <<"dbScene dragged @ " << pos;
    mapPtDst = pos;
    mapPtDst.setY( mapPtDst.y());
    updateTempPath();

    //updateScene();
}

void QMapperDbScene::mouseDropped(QPointF src, QPointF dst)
{
    qDebug() <<"dbScene DROP from " << src <<" to " << dst;

    //undo hovered render on all
    for (int i=0; i<sigs.size(); ++i)
    {
        sigs.at(i)->setHovered(false);
    }
    tempPathItem.setVisible(false);

    QPointF dst_mod(dst.x()+src.x(), dst.y()+src.y());


    int srcIdx = getIndexOfSigNear(src, 10);
    int dstIdx = getIndexOfSigNear(dst_mod, 10);
    qDebug() << "make map from " << srcIdx <<" to " <<dstIdx;
    if ( (srcIdx != -1) & (dstIdx != -1) )
    {
        if (srcIdx != dstIdx) // one more
            addMap(srcIdx, dstIdx);
            activeLayer.addMap(srcIdx, dstIdx);
    }


    //new method using layers:
    activeLayer.setAllHovered(false);



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
    mapPtDst.setY( mapPtDst.y() +src.y()); //SEE TODO BELOW
    mapPtDst.setX( mapPtDst.x() +src.x());

    //find index of source object in list

    //note this is the "modified" point!
    int srcIdx = getIndexOfSigNear(mapPtSrc, 10);

    //then, check if we've moved over an item...

    int hoverIdx = getIndexOfSigNear(mapPtDst, 10);

    //and if so, set it to render with selected graphic

    //new:
    activeLayer.setAllHovered(false);
    activeLayer.setTempHover(srcIdx, hoverIdx);

    //old:
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
            // THE MORE I THINK ABOUT IT THE MORE THIS MAKES SENSE
            //     (e.g. stupid hacks for relative mouse move/drag positions
            //       of child objects betwen source/destination, as above...)
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
    updateTempPath();
    activeLayer.updateTempPath();

    //updateScene();
}

void QMapperDbScene::addMap(int src_idx, int dst_idx)
{
    mapSrcIdxs.push_back(src_idx);
    mapDstIdxs.push_back(dst_idx);
    qDebug() <<"added map from" <<src_idx << " to " << dst_idx;
    updateMapPaths();

    //new:
    activeLayer.addMap(src_idx, dst_idx);
}

void QMapperDbScene::updateTempPath()
{
    //temporary line (while dragging)

    QPainterPath tempPath;
    tempPath.moveTo(mapPtSrc);
    tempPath.cubicTo(mapPtSrc.x()+MAPPER_SCENE_CURVE, mapPtSrc.y(), mapPtDst.x()-MAPPER_SCENE_CURVE, mapPtDst.y(), mapPtDst.x(), mapPtDst.y());
    //tempPath.lineTo(mapPtDst);
    //addPath(tempPath);
    tempPathItem.setPath(tempPath);



}

void QMapperDbScene::updateMapPaths()
{
    //todo: don't need to store these explicitly
    mapSrcs.clear();
    mapDsts.clear();

    for (int i=0; i < mapSrcIdxs.size(); ++i)
    {
        int src_idx = mapSrcIdxs.at(i);
        int dst_idx = mapDstIdxs.at(i);
        QPointF src_pt( (sigs.at(src_idx)->boundingRectAbs().right()),
                                (sigs.at(src_idx)->boundingRectAbs().top() + MAPPER_SCENE_ITEM_H/2));
        QPointF dst_pt( (sigs.at(dst_idx)->boundingRectAbs().left()),
                        (sigs.at(dst_idx)->boundingRectAbs().top() + MAPPER_SCENE_ITEM_H/2));

        mapSrcs.push_back(src_pt);
        mapDsts.push_back(dst_pt);
    }

    //maps list
    for (int i=0; i<mapSrcs.size(); ++i) //no bounds checking at all yet...
    {
        QGraphicsPathItem* pathItem = new QGraphicsPathItem();
        QPainterPath path;
        path.moveTo(mapSrcs.at(i));
        //todo: use the PointF version which looks much neater...
        path.cubicTo(mapSrcs.at(i).x()+MAPPER_SCENE_CURVE, mapSrcs.at(i).y(), mapDsts.at(i).x()-MAPPER_SCENE_CURVE, mapDsts.at(i).y(), mapDsts.at(i).x(), mapDsts.at(i).y());

        pathItem->setPath(path);
        mapPathItems.push_back(pathItem);
        addItem(pathItem);
    }
}

void QMapperDbScene::removeMapPaths()
{
    for (int i=0; i<mapPathItems.size(); ++i)
    {
        removeItem(mapPathItems.at(i));
    }
    while (mapPathItems.size())
    {
        QGraphicsItem* item = mapPathItems.at(mapPathItems.size()-1);
        delete item;
        mapPathItems.pop_back();
    }
}

void QMapperDbScene::redrawMapPaths()
{
    removeMapPaths();
    updateMapPaths();
}

void QMapperDbScene::updateScene()
{
    activeLayer.setMapperDbModel(dbModel);
    activeLayer.updateLayer();

    removeItem(activeLayer.getLayerItems());

    //return;

    //old method, no layer management with just one
    // set of drawings for the entire scene:
    if (dbModel != NULL)
    {
        qDebug() <<"reinit scene from dbModel...";

        //remove stuff
        while (sigs.size())
        {
            removeItem(sigs.at(sigs.size()-1));
            delete sigs.at(sigs.size()-1);
            sigs.pop_back();
        }

        removeMapPaths();

        removeItem(&tempPathItem);//note: find way to avoid this bit
        clear();

        //add stuff back
        addItem(&tempPathItem);
        tempPathItem.setVisible(false);
        sigs.clear();

        addItem(activeLayer.getLayerItems());

        int inputOffsetY = 0;
        int outputOffsetY = 0;
        for (int i=0; i<dbModel->getNumSigs(); ++i)
        {
            QString devname = dbModel->getSigDevName(i);
            QString signame = dbModel->getSigName(i);
            CustomRect * sigrect;
            int offsetY;
            int offsetX;
            if (dbModel->isOutputSig(i))
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
            //QObject::connect(sigrect, SIGNAL(mousePressSig()), this, SLOT(mousePressed()));
            QObject::connect(sigrect, SIGNAL(rectMovedSig()), this, SLOT(devsigMoved()));
            QObject::connect(sigrect, SIGNAL(mouseDoubleClickSig()), this, SLOT(mouseDoubleClicked()));
            sigs.push_back(sigrect);
            addItem(sigrect);

            //load the map objects
        }

        updateMapPaths();
        updateTempPath();
        //updateMaps();

    }
}

void QMapperDbScene::setMapperDbModel(QMapperDbModel* model)
{
    dbModel = model;
    activeLayer.setMapperDbModel(model);
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

void QMapperDbScene::devsigMoved()
{
    qDebug () <<" need to redraw!";
    redrawMapPaths();
    activeLayer.redrawMapPaths();
}

void QMapperDbScene::setAlpha(int alpha)
{
    setForegroundBrush(QColor(255, 255, 255, alpha));
}

#include "qmapperdbscene.h"

QMapperDbScene::QMapperDbScene(QObject *parent) : QGraphicsScene(parent), activeLayer(this, 3)
{
    dbModel = NULL;
    addItem(&tempPathItem);
    tempPathItem.setPen(QPen(Qt::red, 2));
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
    activeLayer.setVisible(!activeLayer.getIsVisible());
    update();
    //updateScene();
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
        if (srcIdx != dstIdx) {// one more
            addMap(srcIdx, dstIdx, true);

            //IF WE WANT TO APPLY ANOTHER LAYER, do this:
            //activeLayer.addMap(srcIdx, dstIdx);
        }
    }
    //new method using layers:
    // (actually other layers are passive, so we'd never do it. but if we ever did...)
    //activeLayer.setAllHovered(false);
}

void QMapperDbScene::mouseDragged(QPointF src, QPointF dst)
{
    //soooo ... many... hacks....

    //Note: lets not do this to other layers, except to "working" layer
    // that interacts with UI.

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

    //IF WE WANT TO APPLY ANOTHER LAYER, do this:
    //activeLayer.setAllHovered(false);
    //activeLayer.setTempHover(srcIdx, hoverIdx);

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

    //IF WE WANT TO APPLY ANOTHER LAYER, do this:
    //activeLayer.updateTempPath();

    //updateScene();
}

void QMapperDbScene::addMap(int src_idx, int dst_idx, bool ifExistsRemove)
{
    int existingIdx = mapExists(src_idx, dst_idx);
    if ( (ifExistsRemove) && ( existingIdx!= -1) ) {

            qDebug() <<"removing existing map # " << existingIdx;
            mapSrcIdxs.erase(mapSrcIdxs.begin()+existingIdx);
            mapDstIdxs.erase(mapDstIdxs.begin()+existingIdx);

    }
    else {
        mapSrcIdxs.push_back(src_idx);
        mapDstIdxs.push_back(dst_idx);
        qDebug() <<"Main Scene added map from" <<src_idx << " to " << dst_idx;
    }

    redrawMapPaths();

    //if we want to add to other layer:
    //activeLayer.addMap(src_idx, dst_idx);
}

int QMapperDbScene::mapExists(int src_idx, int dst_idx)
{
    int found_src = -1;
    for (int i=0; i<mapSrcIdxs.size(); ++i)
    {
        if (mapSrcIdxs.at(i) == src_idx)
        {
            found_src = i;
            if (mapDstIdxs.at(i) == dst_idx)
                return i;
        }
    }
    //not found
    return -1;
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
    //note: in the current implementation we need to remove
    // owned child items explicitly which release ownership
    // of the items from the scene; otherwise we have issues on exit.
    // should fix this by reworking the class composition...

    //also, once the children are put in a group, for some reason
    // the UI signals don't get connected which explains why for now we
    // have almost identical code in this class and the scenelayer class
    // for updating of the scene... ideally a single class should suffice...

    // activeLayer.setMapperDbModel(dbModel);
    activeLayer.updateLayer();
    removeItem(activeLayer.getLayerItems());

    // set of drawings for the entire scene:
    if (dbModel != NULL)
    {
        //qDebug() <<"reinit scene from dbModel...";

        //remove stuff
        while (sigs.size())
        {
            removeItem(sigs.at(sigs.size()-1));
            delete sigs.at(sigs.size()-1);
            sigs.pop_back();
        }
        removeMapPaths();
        removeItem(&tempPathItem);//note: find way to avoid this bit
        clear(); //probably won't need this since we've manually removed everything...

        //add stuff back

        activeLayer.getLayerItems()->setZValue(MAPPER_SCENE_MIDDLE_LAYER);
        activeLayer.setOffset(15);
        activeLayer.setDrawText(false);
        QColor activeLayerCol(200, 200, 0);
        activeLayer.setRectColour(activeLayerCol);
        activeLayer.setAlpha(100);
        addItem(activeLayer.getLayerItems());

        addItem(&tempPathItem);
        tempPathItem.setVisible(false);
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

            sigrect->setFillColor(QColor(128, 128, 128, 255));

            //QObject::connect(sigrect, SIGNAL(mouseDragSig(QPointF)), this, SLOT(mouseDragged(QPointF)));
            QObject::connect(sigrect, SIGNAL(mouseDragSig(QPointF, QPointF)), this, SLOT(mouseDragged(QPointF, QPointF)));
            //QObject::connect(sigrect, SIGNAL(mouseDropSig(QPointF)), this, SLOT(mouseDropped(QPointF)));
            QObject::connect(sigrect, SIGNAL(mouseDropSig(QPointF, QPointF)), this, SLOT(mouseDropped(QPointF, QPointF)));
            //QObject::connect(sigrect, SIGNAL(mousePressSig()), this, SLOT(mousePressed()));
            QObject::connect(sigrect, SIGNAL(rectMovedSig()), this, SLOT(devsigMoved()));
            QObject::connect(sigrect, SIGNAL(mouseDoubleClickSig()), this, SLOT(mouseDoubleClicked()));
            sigs.push_back(sigrect);
            addItem(sigrect);
            sigrect->setZValue(MAPPER_SCENE_BOTTOM_LAYER);


        }

        //load the map objects
        for (int i=0; i<dbModel->getMapSrcs().size(); ++i)
        {
            addMap(dbModel->getMapSrcs().at(i), dbModel->getMapDsts().at(i));
        }


        updateMapPaths();
        updateTempPath();
        //updateMaps();

    }
}

void QMapperDbScene::setMapperDbModel(QMapperDbModel* model)
{
    dbModel = model;

}

void QMapperDbScene::setMapperDbModelActive(QMapperDbModel* model)
{
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

#include "qmapperscenelayer.h"

QMapperSceneLayer::QMapperSceneLayer(QGraphicsScene *parent, int offset)
{
    myParentScene = parent;
    dbModel = NULL;
    tempPathItem = new QGraphicsPathItem();
    tempPathItem->setVisible(false);
    isVisible = true;
    myOffset = offset;
}
QMapperSceneLayer::~QMapperSceneLayer()
{
    removeSigRects();
    removeMapPaths();

}

void QMapperSceneLayer::updateLayer()
{

    if (dbModel != NULL)
    {
        qDebug() <<"updateLayer from dbModel...";

        //
        //remove stuff
        //

        removeSigRects();//for group
        removeMapPaths();
        myGraphicsItems.removeFromGroup(tempPathItem);
        //removeItem(&tempPathItem);//note: find way to avoid this bit


        //
        //add stuff back
        //
        tempPathItem = new QGraphicsPathItem();
        myGraphicsItems.addToGroup(tempPathItem);
        tempPathItem->setVisible(false);
        //addItem(&tempPathItem);



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
            sigrect = new CustomRect(offsetX + myOffset, offsetY+myOffset, devname, signame);
            sigrect->setOutlineColDef(QColor(255, 255, 255, 0));
            //QObject::connect(sigrect, SIGNAL(mouseDragSig(QPointF)), this, SLOT(mouseDragged(QPointF)));
            QObject::connect(sigrect, SIGNAL(mouseDragSig(QPointF, QPointF)), myParentScene, SLOT(mouseDragged(QPointF, QPointF)));
            //QObject::connect(sigrect, SIGNAL(mouseDropSig(QPointF)), this, SLOT(mouseDropped(QPointF)));
            QObject::connect(sigrect, SIGNAL(mouseDropSig(QPointF, QPointF)), myParentScene, SLOT(mouseDropped(QPointF, QPointF)));
            //QObject::connect(sigrect, SIGNAL(mousePressSig()), this, SLOT(mousePressed()));
            QObject::connect(sigrect, SIGNAL(rectMovedSig()), myParentScene, SLOT(devsigMoved()));
            QObject::connect(sigrect, SIGNAL(mouseDoubleClickSig()), myParentScene, SLOT(mouseDoubleClicked()));
            sigs.push_back(sigrect);
            //addItem(sigrect); //scene
            myGraphicsItems.addToGroup(sigrect);
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

void QMapperSceneLayer::redrawMapPaths()
{
    removeMapPaths();
    updateMapPaths();
}

void QMapperSceneLayer::setMapperDbModel(QMapperDbModel* model)
{
    dbModel = model;
}

void QMapperSceneLayer::updateTempPath()
{
    //temporary line (while dragging)

    QPainterPath tempPath;
    tempPath.moveTo(mapPtSrc);
    tempPath.cubicTo(mapPtSrc.x()+MAPPER_SCENE_CURVE, mapPtSrc.y(), mapPtDst.x()-MAPPER_SCENE_CURVE, mapPtDst.y(), mapPtDst.x(), mapPtDst.y());
    //tempPath.lineTo(mapPtDst);
    //addPath(tempPath);
    if (tempPathItem != NULL)
        tempPathItem->setPath(tempPath);
}

void QMapperSceneLayer::addMap(int src_idx, int dst_idx)
{
    mapSrcIdxs.push_back(src_idx);
    mapDstIdxs.push_back(dst_idx);
    qDebug() <<"Layer added map from" <<src_idx << " to " << dst_idx;
    updateMapPaths();
}

void QMapperSceneLayer::updateMapPaths()
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


        //src_pt.operator +=(QPoint(myOffset,myOffset));
        //dst_pt.operator +=(QPoint(myOffset,myOffset));
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
        pathItem->setPen(QPen(myRectColour, 2));
        //pathItem->setBrush(QBrush(myRectColour));
        mapPathItems.push_back(pathItem);
        //addItem(pathItem); //scene

        myGraphicsItems.addToGroup(pathItem);
    }
}

void QMapperSceneLayer::removeMapPaths()
{
    for (int i=0; i<mapPathItems.size(); ++i)
    {
        //removeItem(mapPathItems.at(i)); //scene
        myGraphicsItems.removeFromGroup(mapPathItems.at(i));
    }
    while (mapPathItems.size())
    { //do this in one go remove for above...
        QGraphicsItem* item = mapPathItems.at(mapPathItems.size()-1);
        delete item;
        mapPathItems.pop_back();
    }
}

void QMapperSceneLayer::removeSigRects()
{
    while (sigs.size())
    {
        myGraphicsItems.removeFromGroup(sigs.at(sigs.size()-1));
        delete sigs.at(sigs.size()-1);
        sigs.pop_back();
    }
}

void QMapperSceneLayer::setHovered(int rect_idx, bool is_hover)
{
    sigs.at(rect_idx)->setHovered(is_hover);
}

void QMapperSceneLayer::setAllHovered(bool is_hover)
{
    for (int i=0; i<sigs.size(); ++i)
    {
        sigs.at(i)->setHovered(is_hover);
    }
}

void QMapperSceneLayer::setTempHover(int src_idx, int dst_idx)
{
    for (int i=0; i<sigs.size(); ++i)
    {
        if ( (i == dst_idx) && (src_idx != dst_idx) )
        {
            sigs.at(i)->setHovered(true);
            break;
        }
    }
}

void QMapperSceneLayer::setRectColour(QColor col)
{
    myRectColour = col;
    for (int i=0; i<sigs.size(); ++i)
    {
        sigs.at(i)->setFillColor(myRectColour);
    }
}

void QMapperSceneLayer::setVisible(bool visible)
{
    isVisible = visible;
    myGraphicsItems.setVisible(isVisible);
}
void QMapperSceneLayer::setAlpha(int alpha)
{
    myAlpha = alpha;

    for (int i=0; i<sigs.size(); ++i)
    {
        sigs.at(i)->setAlpha(alpha);
    }
}
void QMapperSceneLayer::setDrawText(bool draw)
{
    for (int i=0; i<sigs.size(); ++i)
    {
        QColor col;
        if (draw)
            col = QColor(Qt::red);
        else
            col = QColor(255, 255, 255, 0);
        sigs.at(i)->setTextColor(col);
    }
}

#ifndef QMAPPERDBSCENE_H
#define QMAPPERDBSCENE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "qmapperdbmodel.h"
#include "customrect.h"

#define MAPPER_SCENE_CURVE  25


class QMapperDbScene : public QGraphicsScene
{
    Q_OBJECT

public:

    QMapperDbScene(QObject *parent);

    void updateScene();

    void redrawScene();

    void setMapperDbModel(QMapperDbModel* model);

    void updateTempPath();

    void addMap(int src_idx, int dst_idx);
    void updateMapPaths();
    void removeMapPaths();

    //void mouseMoveEvent(QGraphicsSceneMouseEvent *e);

public Q_SLOTS:
    void mouseDropped(QPointF pos); //not used any more
    void mouseDropped(QPointF src_pos, QPointF drop_pos);
    void mouseDragged(QPointF pos); //not used any more
    void mouseDragged(QPointF src_pos, QPointF drag_pos);
    void devsigMoved();

private:
    //find index of item within square hitbox of certain length
    int getIndexOfSigNear(QPointF pos, float len);

private:

    QMapperDbModel *dbModel;
    std::vector<CustomRect*> sigs;

    QGraphicsPathItem tempPathItem;
    QPainterPath tempPath;

    QPointF mapPtSrc;
    QPointF mapPtDst;


    std::vector<QGraphicsPathItem*> mapPathItems;
    std::vector<QPointF> mapSrcs;
    std::vector<QPointF> mapDsts;

    std::vector<int> mapSrcIdxs;
    std::vector<int> mapDstIdxs;

};
#endif // QMAPPERDBSCENE_H


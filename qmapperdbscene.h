#ifndef QMAPPERDBSCENE_H
#define QMAPPERDBSCENE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "qmapperdbmodel.h"
#include "customrect.h"
#include "qmapperscenelayer.h"


//for Z-order of objects
#define MAPPER_SCENE_TOP_LAYER 30
#define MAPPER_SCENE_MIDDLE_LAYER 20
#define MAPPER_SCENE_BOTTOM_LAYER 10

class QMapperDbScene : public QGraphicsScene
{
    Q_OBJECT

public:

    QMapperDbScene(QObject *parent);
    ~QMapperDbScene();

    void updateScene();

    void redrawMapPaths();

    void setMapperDbModel(QMapperDbModel* model);
    void setMapperDbModelActive(QMapperDbModel* model);

    void updateTempPath();

    void addMap(int src_idx, int dst_idx, bool ifExistsRemove = false);
    int mapExists(int src_idx, int dst_idx);
    void updateMapPaths();
    void removeMapPaths();
    //void mouseMoveEvent(QGraphicsSceneMouseEvent *e);

    void setAlpha(int alpha);

public Q_SLOTS:
    void mouseDropped(QPointF pos); //not used any more
    void mouseDropped(QPointF src_pos, QPointF drop_pos);
    void mouseDragged(QPointF pos); //not used any more
    void mouseDragged(QPointF src_pos, QPointF drag_pos);
    void devsigMoved();
    void mouseDoubleClicked();

private:
    //find index of item within square hitbox of certain length
    int getIndexOfSigNear(QPointF pos, float len);

private:

    QMapperDbModel *dbModel;
    std::vector<CustomRect*> sigs;

    //draw items for temp line
    QGraphicsPathItem tempPathItem;
    QPainterPath tempPath;
    QPointF mapPtSrc;
    QPointF mapPtDst;


    //draw items for map list
    std::vector<QGraphicsPathItem*> mapPathItems;
    std::vector<QPointF> mapSrcs;
    std::vector<QPointF> mapDsts;

    //list of map src/dsts
    std::vector<int> mapSrcIdxs;
    std::vector<int> mapDstIdxs;

    QMapperSceneLayer activeLayer;

};
#endif // QMAPPERDBSCENE_H


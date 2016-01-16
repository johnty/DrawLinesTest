#ifndef QMAPPERDBSCENE_H
#define QMAPPERDBSCENE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "qmapperdbmodel.h"
#include "customrect.h"

#define MAPPER_SCENE_ITEM_H 35
#define MAPPER_SCENE_ITEM_W 45
#define MAPPER_SCENE_SPACER 5

class QMapperDbScene : public QGraphicsScene
{
    Q_OBJECT

public:

    QMapperDbScene(QObject *parent);

    void updateScene();

    void setMapperDbModel(QMapperDbModel* model);

    void updateMaps();

    //void mouseMoveEvent(QGraphicsSceneMouseEvent *e);

public Q_SLOTS:
    void mouseDropped(QPointF pos);
    void mouseDropped(QPointF src_pos, QPointF drop_pos);
    void mouseDragged(QPointF pos);
    void mouseDragged(QPointF src_pos, QPointF drag_pos);



private:

    QMapperDbModel *dbModel;
    std::vector<CustomRect*> sigs;
    std::vector<QGraphicsPathItem*> maps;
    QGraphicsPathItem tempPathItem;
    QPainterPath tempPath;

    QPointF mapPtSrc;
    QPointF mapPtDst;

};
#endif // QMAPPERDBSCENE_H


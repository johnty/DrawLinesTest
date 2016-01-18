#ifndef QMAPPERSCENELAYER_H
#define QMAPPERSCENELAYER_H
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include "customrect.h"
#include "qmapperdbmodel.h"

#define MAPPER_SCENE_CURVE  25

class QMapperSceneLayer
{
public:
    QMapperSceneLayer(QGraphicsScene* parent);
    ~QMapperSceneLayer();

    void updateLayer();

    void redrawMapPaths();

    void setMapperDbModel(QMapperDbModel* model);

    void updateTempPath();

    void addMap(int src_idx, int dst_idx);
    void updateMapPaths();
    void removeMapPaths();

    void removeSigRects();

    void setHovered(int rect_idx, bool is_hover);
    void setAllHovered(bool is_hover);
    void setTempHover(int src_idx, int dst_idx);

    QGraphicsItemGroup* getLayerItems() {return &myGraphicsItems;}

private:

    QMapperDbModel *dbModel;
    std::vector<CustomRect*> sigs;

    QGraphicsPathItem *tempPathItem;
    QPainterPath tempPath;

    QPointF mapPtSrc;
    QPointF mapPtDst;

    std::vector<QGraphicsPathItem*> mapPathItems;
    std::vector<QPointF> mapSrcs;
    std::vector<QPointF> mapDsts;

    std::vector<int> mapSrcIdxs;
    std::vector<int> mapDstIdxs;

    QGraphicsItemGroup myGraphicsItems;
    QGraphicsScene* myParentScene;
};

#endif // QMAPPERSCENELAYER_H

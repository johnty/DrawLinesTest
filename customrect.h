#ifndef CUSTOMRECT_H
#define CUSTOMRECT_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QObject>
//#include "qmapperdbscene.h"


class CustomRect : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:

    CustomRect(float x, float y, QString dev, QString sig);// QMapperDbScene* scene = 0);

    void setTitles(QString dev, QString sig);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

Q_SIGNALS:

    void mouseDragSig(QPointF pos);
    void mouseDragSig(QPointF src_pos, QPointF drag_pos);
    void mouseDropSig(QPointF pos);
    void mouseDropSig(QPointF src_pos, QPointF drop_pos);
    void mouseReleaseEvent(QPointF pos);

protected:

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);

    void mousePressEvent(QGraphicsSceneMouseEvent *e);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

    void mouseMoveEvent(QGraphicsSceneMouseEvent *e);


    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    bool lifted;
    bool pressed;
    QString devname;
    QString signame;
    float originX;
    float originY;

    //QMapperDbScene *parentScene;

};

#endif // CUSTOMRECT_H

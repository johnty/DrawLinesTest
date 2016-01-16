#include "customrect.h"


CustomRect::CustomRect(float x, float y, QString dev, QString sig)// QMapperDbScene *scene) {
{
    devname = dev;
    signame = sig;
    originX = x;
    originY = y;
    setPos(originX, originY);
    setFlags(ItemSendsGeometryChanges | ItemIsMovable);
    pressed = false;
    lifted = false;
    //parentScene = scene;
}

void CustomRect::setTitles(QString dev, QString sig) {
    devname = dev;
    signame = sig;
}

QRectF CustomRect::boundingRect() const {
    return QRectF(0, 0, 120, 30);
}

void CustomRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QRectF rect = boundingRect();
    QPen pen;
    pen.setWidth(2);
    QBrush brush (Qt::blue);
    if (pressed)
        pen.setColor(Qt::red);
    else
        pen.setColor(Qt::black);
    painter->setPen(pen);
    painter->fillRect(rect,brush);
    painter->drawRect(rect);

    painter->setPen(Qt::green);
    QString text = devname + "/" + signame;
    painter->drawText(rect, text);

}

void CustomRect::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e)
{

}

void CustomRect::mousePressEvent(QGraphicsSceneMouseEvent *e) {
    if (e->button() == Qt::LeftButton)
    {
        pressed = true;
        qDebug()<<"press = true";
    }
    else
    {
        qDebug()<<"lift = true";
        lifted = true;

    }
    QGraphicsItem::mousePressEvent(e);
    update();
}

void CustomRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *e) {
    qDebug() <<"mRelease";
    if (e->button() == Qt::LeftButton)
    {
        if (pressed)
        {   //a release... tell parent to see if we made connection with anything...
            pressed = false;
            qDebug()<<"press = false";
            Q_EMIT mouseDropSig(e->pos());
            //parentScene->mouseDropped(e->pos());

        }
    }
    else
    {
        qDebug()<<"lift = false";
        lifted = false;
        //setFlags(ItemSendsGeometryChanges);
        //setFlags(ItemSendsGeometryChanges | ItemIsMovable);
    }


    QGraphicsItem::mouseReleaseEvent(e);
    update();
}

void CustomRect::mouseMoveEvent(QGraphicsSceneMouseEvent *e) {
    if (lifted)
    {
        //looks like a hack, probably is a hack.

        //manual move:
        qDebug() << " pos = " << e->pos() <<"; lastpos = " << e->lastPos();
        QPointF diff = e->lastPos() - e->pos();
        this->setPos(this->pos() - diff);

        update();
    }

    qDebug() <<"customrect mousemove:  "<< e->pos();

    if (pressed) //dragging action for "map": need to tell parent to draw arrow!
    {
        Q_EMIT mouseDragSig(e->pos());
    }

    //we don't use the default move behavior, otherwise
    // it will always be left click drag to move...
    //QGraphicsItem::mouseMoveEvent(e);
}

QVariant CustomRect::itemChange(GraphicsItemChange change, const QVariant &value) {

    if (change == ItemPositionChange) {
        originX = value.toPointF().x();
        originY = value.toPointF().y();
        qDebug() << "X = " << originX << " Y = " << originY;
    }
    return QGraphicsItem::itemChange(change, value);
}

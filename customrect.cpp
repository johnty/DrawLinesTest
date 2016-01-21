#include "customrect.h"


CustomRect::CustomRect(float x, float y, QString dev, QString sig)// QMapperDbScene *scene) {
{
    devname = dev;
    signame = sig;
    originX = x;
    originY = y;
    setPos(originX, originY);
    //setFlags(ItemSendsGeometryChanges | ItemIsMovable);
    setFlags(ItemSendsGeometryChanges);
    pressed = false;
    lifted = false;
    hoverovered = false;
    setParentItem(0);

    colorFill = Qt::gray;
    colorText = Qt::blue;
}

void CustomRect::setTitles(QString dev, QString sig) {
    devname = dev;
    signame = sig;
}

QRectF CustomRect::boundingRect() const {
    return QRectF(0, 0, MAPPER_SCENE_ITEM_W, MAPPER_SCENE_ITEM_H);
}

QRectF CustomRect::boundingRectAbs() const {
    return QRectF(originX, originY, MAPPER_SCENE_ITEM_W, MAPPER_SCENE_ITEM_H);
}

void CustomRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QRectF rect = boundingRect();
    QPen pen;
    pen.setWidth(2);
    QBrush brush (colorFill);

    //pen.setColor(Qt::black);
    if (pressed)
        pen.setColor(Qt::red);
    if (hoverovered)
        pen.setColor(Qt::green);

    //painter->setPen(pen,)
    painter->fillRect(rect,brush);
    painter->drawRect(rect);

    painter->setPen(colorText);
    QString text = devname + "/" + signame;
    rect.moveTo(2,2);
    painter->drawText(rect, text);

}

void CustomRect::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e)
{
    Q_EMIT mouseDoubleClickSig();
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
    //QGraphicsItem::mousePressEvent(e);
    update();
}

void CustomRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *e) {
    qDebug() <<"customrect mRelease";
    if (e->button() == Qt::LeftButton)
    {
        if (pressed)
        {   //a release... tell parent to see if we made connection with anything...
            pressed = false;
            qDebug()<<"press = false";
            //Q_EMIT mouseDropSig(e->pos());
            Q_EMIT mouseDropSig(this->pos(), e->pos());


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

        Q_EMIT rectMovedSig();
        update();
    }

    qDebug() <<"customrect mousemove:  "<< e->pos();

    if (pressed) //dragging action for "map": need to tell parent to draw arrow!
    {
        //Q_EMIT mouseDragSig(e->pos());

        Q_EMIT mouseDragSig(this->pos(), e->pos());
    }

    QGraphicsItem::mouseMoveEvent(e);
}

QVariant CustomRect::itemChange(GraphicsItemChange change, const QVariant &value) {

    if (change == ItemPositionChange) {
        originX = value.toPointF().x();
        originY = value.toPointF().y();
        qDebug() << "X = " << originX << " Y = " << originY;
    }
    return QGraphicsItem::itemChange(change, value);
}

void CustomRect::setAlpha(int alpha)
{
    colorFill.setAlpha(alpha);
    colorOutlineDef.setAlpha(alpha);
}

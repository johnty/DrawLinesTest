#include "listviewarrows.h"
#include <QDebug>
#include <QPainter>

ListViewArrows::ListViewArrows(QWidget* parent) : QListView(parent)
{

}

void ListViewArrows::paintEvent(QPaintEvent* event)
{
    //call parent paint event
    QListView::paintEvent(event);
    qDebug() << "paintEvent called";

    QPainter painter(this->viewport());
    painter.setPen(QColor::fromRgb(255, 0, 0));

    //painter.fillRect(0, 0, 10, 10, Qt::BrushStyle::);

    int rowH = 22;
    int firstRowH = 17;

    int width = this->viewport()->size().width();
    //try some "connections"

    //first item -> first item

    int startY = firstRowH;
    int lineN = 1;
    startY+=  lineN * rowH/2;
    painter.drawLine(0, startY, width, startY );

    //first item-> second item
    int endY = startY + rowH;
    painter.drawLine(0, startY, width, endY );



    //use path and cubicTo
    QPainterPath *path = new QPainterPath();
//    float xL = ui->links->viewport()->rect().left();
//    float xR = ui->links->viewport()->rect().right();


    startY = firstRowH + 2.5 * rowH;
    endY = firstRowH + 4.5* rowH;
    path->moveTo(0, startY);
    path->cubicTo(0+width/4, startY, width*0.75, endY, width, endY);
    painter.drawPath(*path);


    startY = firstRowH + 7.5 * rowH;
    endY = firstRowH + 1.5* rowH;
    path->moveTo(0, startY);
    path->cubicTo(0+width/4, startY, width*0.75, endY, width, endY);
    painter.drawPath(*path);



    //painter.drawRect(0,0,this->viewport()->width(), 20);




}


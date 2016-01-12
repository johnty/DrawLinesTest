#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class CustomRect : public QGraphicsItem
{
public:

    CustomRect(float x, float y, QString title) {
        mytext = title;
        originX = x;
        originY = y;
        setPos(originX, originY);
        setFlags(flags() | ItemIsMovable | ItemSendsGeometryChanges);
    }

    void setTitle(QString text) { mytext = text; }

    QRectF boundingRect() const {
        return QRectF(0, 0, 120, 30);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
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
        painter->drawText(rect, mytext);

    }
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *e) {
        pressed = true;
        QGraphicsItem::mousePressEvent(e);
        update();
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *e) {
        pressed = false;
        QGraphicsItem::mouseReleaseEvent(e);
        update();
    }

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) {

        if (change == ItemPositionChange) {
            originX = value.toPointF().x();
            originY = value.toPointF().y();
            qDebug() << "X = " << originX << " Y = " << originY;
        }
        return QGraphicsItem::itemChange(change, value);
    }

private:
    bool pressed;
    QString mytext;
    float originX;
    float originY;

};

class MapperDbScene : public QGraphicsScene
{

};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_listView_entered(const QModelIndex &index);
    void onScrolled();

private:
    QStandardItemModel listSrcTree;
    QStandardItemModel listDstTree;
    QStandardItemModel listArrows;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

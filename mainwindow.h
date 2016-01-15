#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QDebug>
#include "qmapperdbmodel.h"

namespace Ui {
class MainWindow;
}

class CustomRect : public QGraphicsItem
{
public:

    CustomRect(float x, float y, QString dev, QString sig) {
        devname = dev;
        signame = sig;
        originX = x;
        originY = y;
        setPos(originX, originY);
        setFlags(flags() | ItemIsMovable | ItemSendsGeometryChanges);
    }

    void setTitles(QString dev, QString sig) {
        devname = dev;
        signame = sig;
    }

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
        QString text = devname + "/" + signame;
        painter->drawText(rect, text);

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
    QString devname;
    QString signame;
    float originX;
    float originY;

};

class QMapperDbScene : public QGraphicsScene
{ // when separated, include qmappermodel.h...
    /*
     #include "qmapperdbmodel.h"
     #include "customrect.h"
     #include "
      */
public:

    QMapperDbScene(QObject *parent) : QGraphicsScene(parent)
    {
        dbModel = NULL;
    }


    void updateScene()
    {
        if (dbModel != NULL)
        {
            clear();
            sigs.clear();

            for (int i=0; i<dbModel->getNumSigs(); ++i)
            {
                QString devname = dbModel->getSigDevName(i);
                QString signame = dbModel->getSigName(i);
                int Yoffset = i *35;
                CustomRect * sigrect = new CustomRect(0, Yoffset, devname, signame);
                sigs.push_back(sigrect);
                addItem(sigrect);
            }

        }
    }

    void setMapperDbModel(QMapperDbModel* model)
    {
        dbModel = model;
    }



private:
    QMapperDbModel *dbModel;
    std::vector<CustomRect*> sigs;

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
    QMapperDbScene * mapperScene;
    QMapperDbModel * dbModel;
    QStandardItemModel listSrcTree;
    QStandardItemModel listDstTree;
    QStandardItemModel listArrows;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

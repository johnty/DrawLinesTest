#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    int rowH = 22;

    listSrcTree.setHorizontalHeaderItem(0, new QStandardItem("Src Devices"));
    QStandardItem* rootItem = listSrcTree.invisibleRootItem();
    QStandardItem* dev1 = new QStandardItem("Device 1");
    dev1->setSizeHint(QSize(20, rowH));
    dev1->appendRow(new QStandardItem("sensor 0"));
    dev1->child(dev1->rowCount()-1)->setSizeHint(QSize(20, rowH));
    dev1->appendRow(new QStandardItem("sensor 1"));
    dev1->child(dev1->rowCount()-1)->setSizeHint(QSize(20, rowH));

    QStandardItem* dev2 = new QStandardItem("Device 2");
    dev2->setSizeHint(QSize(20, rowH));
    dev2->appendRow(new QStandardItem("sensor 0"));
    dev2->child(dev2->rowCount()-1)->setSizeHint(QSize(20, rowH));
    dev2->appendRow(new QStandardItem("sensor 1"));
    dev2->child(dev2->rowCount()-1)->setSizeHint(QSize(20, rowH));
    dev2->appendRow(new QStandardItem("sensor 2"));
    dev2->child(dev2->rowCount()-1)->setSizeHint(QSize(20, rowH));
    dev2->appendRow(new QStandardItem("sensor 3"));
    dev2->child(dev2->rowCount()-1)->setSizeHint(QSize(20, rowH));
    rootItem->appendRow(dev1);
    rootItem->appendRow(dev2);

    listDstTree.setHorizontalHeaderItem(0, new QStandardItem("Dst Devices"));
    rootItem = listDstTree.invisibleRootItem();
    QStandardItem* dev3 = new QStandardItem("Device 3");
    dev3->setSizeHint(QSize(20, rowH));
    dev3->appendRow(new QStandardItem("actuator 0"));
    dev3->child(dev3->rowCount()-1)->setSizeHint(QSize(20, rowH));
    dev3->appendRow(new QStandardItem("actuator 1"));
    dev3->child(dev3->rowCount()-1)->setSizeHint(QSize(20, rowH));
    QStandardItem* dev4 = new QStandardItem("Device 4");
    dev4->setSizeHint(QSize(20, rowH));
    dev4->appendRow(new QStandardItem("actuator 0"));
    dev4->child(dev4->rowCount()-1)->setSizeHint(QSize(20, rowH));
    dev4->appendRow(new QStandardItem("actuator 1"));
    dev4->child(dev4->rowCount()-1)->setSizeHint(QSize(20, rowH));
    dev4->appendRow(new QStandardItem("actuator 2"));
    dev4->child(dev4->rowCount()-1)->setSizeHint(QSize(20, rowH));

    QStandardItem* dev5 = new QStandardItem("Device 5");
    dev5->setSizeHint(QSize(20, rowH));
    for (int i=0; i<10; ++i)
    {
        QString name = "actuator " + QString::number(i);
        dev5->appendRow(new QStandardItem(name));
        dev5->child(dev5->rowCount()-1)->setSizeHint(QSize(20, rowH));
    }
    rootItem->appendRow(dev3);
    rootItem->appendRow(dev4);
    rootItem->appendRow(dev5);

    ui->treeView1->setModel(&listSrcTree);
    ui->treeView2->setModel(&listDstTree);

    ui->treeView1->expandAll();
    ui->treeView2->expandAll();


    //
    ui->listViewArrows->setModel(&listArrows);
    //ui->listViewArrows->sizeHintForRow(25);
    //ui->listViewArrows->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    rootItem = listArrows.invisibleRootItem();
    qDebug() << "item size = " << rootItem->sizeHint();
    listArrows.setHorizontalHeaderItem(0, new QStandardItem("Mappings"));

    QStandardItem* item = new QStandardItem("Maps");

    //NOTE: tree view header is 17...
    // hackety hack...
    item->setSizeHint(QSize(30, 17));
    rootItem->appendRow(item);
    rootItem->appendRow(new QStandardItem("Maps"));
    rootItem->child(rootItem->rowCount()-1)->setSizeHint(QSize(30, rowH));
    rootItem->appendRow(new QStandardItem("Maps"));
    rootItem->child(rootItem->rowCount()-1)->setSizeHint(QSize(30, rowH));
    rootItem->appendRow(new QStandardItem("Maps"));
    rootItem->child(rootItem->rowCount()-1)->setSizeHint(QSize(30, rowH));
    rootItem->appendRow(new QStandardItem("Maps"));
    rootItem->child(rootItem->rowCount()-1)->setSizeHint(QSize(30, rowH));
    rootItem->appendRow(new QStandardItem("Maps"));
    rootItem->child(rootItem->rowCount()-1)->setSizeHint(QSize(30, rowH));



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listView_entered(const QModelIndex &index)
{

}
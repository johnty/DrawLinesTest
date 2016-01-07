#ifndef LISTVIEWARROWS_H
#define LISTVIEWARROWS_H
#include <QListView>
#include <QStandardItemModel>


class ListViewArrows: public QListView
{
public:
    ListViewArrows(QWidget* parent);
    void paintEvent(QPaintEvent* event);
    void wheelEvent(QWheelEvent* event);

private:
};

#endif // LISTVIEWARROWS_H

#ifndef LISTVIEWARROWS_H
#define LISTVIEWARROWS_H
#include <QListView>
#include <QStandardItemModel>


class ListViewArrows: public QListView
{
public:
    ListViewArrows(QWidget* parent);
    void paintEvent(QPaintEvent* event);

private:
};

#endif // LISTVIEWARROWS_H

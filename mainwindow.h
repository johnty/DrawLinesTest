#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

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

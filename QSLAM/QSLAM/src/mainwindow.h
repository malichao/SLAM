#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

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
    void on_actionOpen_triggered();

    void on_actionSetting_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    //QGraphicsView *view;
    QScrollArea *scrollArea;
    QGraphicsPixmapItem *backgroundImage;
    QPixmap *map;

};

#endif // MAINWINDOW_H

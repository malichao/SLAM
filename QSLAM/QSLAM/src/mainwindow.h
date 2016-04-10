#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <vector>

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

    void on_buttonSetStart_clicked();

    void on_buttonSetTarget_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    //QGraphicsView *view;
    QScrollArea *scrollArea;
    QGraphicsPixmapItem *backgroundImage;
    QGraphicsPixmapItem *startImage;
    QGraphicsPixmapItem *targetImage;
    QPixmap *mapImage;
    bool setStartPressed;
    bool setTargetPressed;
    QLabel *coordinateLabel;
    QPoint start,target;

    std::vector<std::vector<bool> > *map;

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *event);
};

#endif // MAINWINDOW_H

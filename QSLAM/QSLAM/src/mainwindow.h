#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "settingdialog.h"

#include <vector>
#include "RRT.h"
#include "ui_mainwindow.h"

class QLabel;

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

    void on_actionSearch_triggered();

    void on_actionDrive_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    //QGraphicsView *view;
    QScrollArea *scrollArea;
    QGraphicsPixmapItem *backgroundImage;
    QGraphicsPixmapItem *startImage;
    QGraphicsPixmapItem *targetImage;
    QGraphicsPixmapItem *carImage;
    double carImageScale;
    QPixmap *mapImage;
    bool setStartPressed;
    bool setTargetPressed;
    QLabel *coordinateLabel;
    SearchAlgorithms::Point_uint start,target;
    bool renewSetPoint;
    bool renewStart;
    bool renewTarget;

    std::vector<std::vector<bool> > map;
    QVector<QGraphicsLineItem*> lineItems;
    std::vector<SearchAlgorithms::Point_uint> route;
    std::vector<SearchAlgorithms::RRTSearch::Node> animationRoute;

    bool driveAnimationFlag;
    int driveAnimationIndex;
    int animationCount;
    int animationPeriod;

    SettingDialog *settingDialog;

    void mousePressEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent *event);
};

#endif // MAINWINDOW_H

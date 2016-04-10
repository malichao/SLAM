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

    void on_buttonSetStart_clicked();

    void on_buttonSetTarget_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    //QGraphicsView *view;
    QScrollArea *scrollArea;
    QGraphicsPixmapItem *backgroundImage;
    QPixmap *map;
    bool setStartPressed;
    bool setTargetPressed;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *event);
    QLabel *coordinateLabel;
};

#endif // MAINWINDOW_H

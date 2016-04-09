#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene= new QGraphicsScene();
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    ui->view->setScene(scene);
    ui->view->setRenderHint(QPainter::Antialiasing);
    ui->view->setCacheMode(QGraphicsView::CacheBackground);
    ui->view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->view->setDragMode(QGraphicsView::ScrollHandDrag);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Map"), "",
                  tr("BMP (*.bmp);;JPEG (*.jpg);;All types (*.*)"));
    if(fileName==NULL)
        return;
    map=new QPixmap(fileName);
    backgroundImage =new QGraphicsPixmapItem(*map);
    scene->addItem(backgroundImage);
}

void MainWindow::on_actionSetting_triggered()
{

}

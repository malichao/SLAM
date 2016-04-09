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
    //scene->setSceneRect(0,0,1000,1000);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    // Show view on main window
    view=new QGraphicsView(this->centralWidget());
    view->setScene(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    //view->resize(400, 300);

    scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(view);
    setCentralWidget(scrollArea);
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
    QPixmap image(fileName);
    view->setBackgroundBrush(image);
    view->resize(QSize(image.width()+5,image.height()+5));
    scene->setSceneRect(0,0,image.width(),image.height());
    view->show();
    scrollArea->setWidgetResizable(true);
}

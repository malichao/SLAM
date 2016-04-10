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
    //ui->view->setDragMode(QGraphicsView::ScrollHandDrag);

    setStartPressed=false;
    setTargetPressed=false;

    coordinateLabel=new QLabel(this);
    //coordinateLabel->setAlignment(Qt::AlignLeft);
    ui->statusBar->addPermanentWidget(coordinateLabel);
    startTimer(50);
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
    //backgroundImage->setPos();
    scene->addItem(backgroundImage);

}

void MainWindow::on_actionSetting_triggered()
{

}

void MainWindow::on_buttonSetStart_clicked()
{
    if(!setStartPressed)
        setStartPressed=true;
    ui->view->setCursor(Qt::PointingHandCursor);
    ui->buttonSetStart->setEnabled(false);
    ui->buttonSetTarget->setEnabled(false);
}

void MainWindow::on_buttonSetTarget_clicked()
{
    if(!setTargetPressed)
        setTargetPressed=true;
    ui->view->setCursor(Qt::PointingHandCursor);
    ui->buttonSetStart->setEnabled(false);
    ui->buttonSetTarget->setEnabled(false);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(setStartPressed||setTargetPressed){

        // Convert the mouse position to scene position
        QPoint p1=ui->view->mapFromGlobal(QCursor::pos());
        QPointF p=ui->view->mapToScene(p1);

        // Sanity Check
        if((p.x()<0)||(p.x()>map->width())||(p.y()<0)||(p.y()>map->height())){
            QString message("Set point must be whithin the map.");
            QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),message, 0, this);
            msgBox.addButton(tr("Continue"), QMessageBox::AcceptRole);
            msgBox.exec();

            setStartPressed=false;
            setTargetPressed=false;
            ui->buttonSetStart->setEnabled(true);
            ui->buttonSetTarget->setEnabled(true);
            return;
        }

        if(setStartPressed)
            scene->addEllipse(p.x(),p.y(),5,5,QPen(Qt::red),QBrush());
        else if(setTargetPressed)
            scene->addEllipse(p.x(),p.y(),5,5,QPen(Qt::blue),QBrush());

        // Enable button again
        setStartPressed=false;
        setTargetPressed=false;
        ui->buttonSetStart->setEnabled(true);
        ui->buttonSetTarget->setEnabled(true);
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    QPoint p1=ui->view->mapFromGlobal(QCursor::pos());
    QPointF p=ui->view->mapToScene(p1);
    QString s="("+QString::number(p.x(),'f',3)+","+QString::number(p.y(),'f',3)+")";
    coordinateLabel->setText(s);

}


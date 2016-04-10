#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QHBoxLayout>

#include "RRT.h"

/*
 * Be careful,in Qt framework the coordinate is like this:
 *  map[x][y]
 *  0------->x
 *  |
 *  |
 *  |
 *  v
 *  y
 *
 * While in my SearchAlgorithms implementation,the coordinate is like this:
 *  map[x][y]:
 *  0-------> y
 *  |
 *  |
 *  v
 *  x
 *
 * */



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene= new QGraphicsScene();
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setBackgroundBrush(QBrush(QColor(237,237,237)));
    //scene->setBackgroundBrush(QBrush(QColor(195,195,195)));

    ui->view->setScene(scene);
    ui->view->setRenderHint(QPainter::Antialiasing);
    ui->view->setCacheMode(QGraphicsView::CacheBackground);
    ui->view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    //ui->view->setDragMode(QGraphicsView::ScrollHandDrag);

    setStartPressed=false;
    setTargetPressed=false;
    ui->buttonSetStart->setIcon(QIcon(":/icon/resources/start.ico"));
    ui->buttonSetTarget->setIcon(QIcon(":/icon/resources/target.ico"));
    ui->buttonSetStart->setEnabled(false);
    ui->buttonSetTarget->setEnabled(false);

    ui->actionSearch->setIcon(QIcon(":/icon/resources/search.ico"));
    ui->actionOpen->setIcon(QIcon(":/icon/resources/open.ico"));
    ui->actionSearch->setEnabled(false);
    renewSetPoint=false;

    startImage=new QGraphicsPixmapItem(QPixmap(":/image/resources/start.png"));
    targetImage=new QGraphicsPixmapItem(QPixmap(":/image/resources/target.png"));

    this->setWindowIcon(QIcon(":/icon/resources/nascar_racing_car.ico"));

    coordinateLabel=new QLabel(this);
    //coordinateLabel->setAlignment(Qt::AlignLeft);
    ui->statusBar->addPermanentWidget(coordinateLabel);
    startTimer(50);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    delete backgroundImage;
    delete startImage;
    delete targetImage;
    delete mapImage;
    delete coordinateLabel;
    delete map;
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Map"), "",
                  tr("BMP (*.bmp);;JPEG (*.jpg);;All types (*.*)"));
    if(fileName==NULL)
        return;
    mapImage=new QPixmap(fileName);
    backgroundImage =new QGraphicsPixmapItem(*mapImage);
    //backgroundImage->setPos();
    scene->addItem(backgroundImage);

    using namespace std;
    map=new vector<vector<bool> >(mapImage->height(),vector<bool>(mapImage->width(),true));

    //Convert QPixmap to QImage for IO manipulation
    QImage image=mapImage->toImage();
    for(size_t i=0;i<mapImage->width();i++)
        for(size_t j=0;j<image.height();j++){
            QColor rgb=image.pixel(i,j);  //black 0 white 255
            (*map)[j][i]= rgb.red()+rgb.green()+rgb.blue() ==255*3 ? true:false;
        }

    // Enable the buttons
    ui->buttonSetStart->setEnabled(true);
    ui->buttonSetTarget->setEnabled(true);
    scene->removeItem(startImage);
    scene->removeItem(targetImage);

    renewStart=false;
    renewTarget=false;
    renewSetPoint=renewStart&renewTarget;
    ui->actionSearch->setEnabled(renewSetPoint);
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
        if((p.x()<0)||(p.x()>mapImage->width())||(p.y()<0)||(p.y()>mapImage->height())){
            QString message("Set point must be whithin the map.");
            QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),message, 0, this);
            msgBox.addButton(tr("Continue"), QMessageBox::AcceptRole);
            msgBox.exec();
            setStartPressed=false;
            setTargetPressed=false;
        }

        if(setStartPressed){
            // Offset the flag
            p.setX(p.x()-25);
            p.setY(p.y()-80);
            startImage->setPos(p);
            scene->addItem(startImage);
            setStartPressed=false;
            renewStart=true;
        }
        else if(setTargetPressed){
            // Offset the flag
            p.setX(p.x()-5);
            p.setY(p.y()-80);
            targetImage->setPos(p);
            scene->addItem(targetImage);
            setTargetPressed=false;
            renewTarget=true;
        }

        renewSetPoint=renewStart&renewTarget;
        ui->actionSearch->setEnabled(renewSetPoint);

        // Enable button again
        setStartPressed=false;
        setTargetPressed=false;
        ui->buttonSetStart->setEnabled(true);
        ui->buttonSetTarget->setEnabled(true);
        ui->view->setCursor(Qt::ArrowCursor);
        return;
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    QPoint p1=ui->view->mapFromGlobal(QCursor::pos());
    QPointF p=ui->view->mapToScene(p1);
    QString s="("+QString::number(p.x(),'f',3)+","+QString::number(p.y(),'f',3)+")";
    coordinateLabel->setText(s);

}


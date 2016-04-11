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
    ui->actionDrive->setIcon(QIcon(":/icon/resources/drive.ico"));
    ui->actionSearch->setEnabled(false);
    ui->actionDrive->setEnabled(false);
    renewSetPoint=false;

    startImage=new QGraphicsPixmapItem(QPixmap(":/image/resources/start.png"));
    targetImage=new QGraphicsPixmapItem(QPixmap(":/image/resources/target.png"));

    this->setWindowIcon(QIcon(":/icon/resources/nascar_racing_car.ico"));

    ui->actionSetting->setIcon(QIcon(":/icon/resources/wrench.ico"));
    settingDialog=new SettingDialog();


    lineItems=new QVector<QGraphicsLineItem*>();

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

    scene->clear();
    lineItems->clear();

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
    startImage=new QGraphicsPixmapItem(QPixmap(":/image/resources/start.png"));
    targetImage=new QGraphicsPixmapItem(QPixmap(":/image/resources/target.png"));
    carImage=new QGraphicsPixmapItem(QPixmap(":/image/resources/car.png"));
    carImageScale=settingDialog->setting().carScaleRatio/584.0;
    carImage->setScale(carImageScale);

    renewStart=false;
    renewTarget=false;
    renewSetPoint=renewStart&renewTarget;
    ui->actionSearch->setEnabled(renewSetPoint);
    ui->actionDrive->setEnabled(renewSetPoint);
}

void MainWindow::on_actionSetting_triggered()
{
    settingDialog->exec();
    carImageScale=settingDialog->setting().carScaleRatio/584.0;
    carImage->setScale(carImageScale);
    scene->addItem(carImage);
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
            start.y=p.x();
            start.x=p.y();

            // Offset the flag
            p.setX(p.x()-25);
            p.setY(p.y()-80);
            startImage->setPos(p);
            scene->addItem(startImage);
            carImage->setPos(start.y-129*carImageScale,start.x-292*carImageScale);
            scene->addItem(carImage);
            setStartPressed=false;
            renewStart=true;
        }
        else if(setTargetPressed){
            target.y=p.x();
            target.x=p.y();

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


void MainWindow::on_actionSearch_triggered()
{
    if(map==NULL)
        return;

    ui->actionSearch->setEnabled(false);

    using namespace SearchAlgorithms;
    RRTSearch tester;
    tester.setScale(settingDialog->setting().carScaleRatio);
    std::vector<Point_uint> route;
    tester.searchUsingVehicle(*map,start,target,route);

    QPen pen(1);
    pen.setColor(QColor(0,162,232));

    // First clear the last result in the scene
    for(int i=0;i<lineItems->size();i++){
        scene->removeItem((*lineItems)[i]);
    }
    lineItems->resize(tester.getLineSize()+route.size());

    // First draw all the search attempts
    for(size_t i=0;i<tester.getLineSize();i++){
        RRTSearch::Line l=tester.getLine(i);
        (*lineItems)[i]=(new QGraphicsLineItem(l.start.y,l.start.x,l.end.y,l.end.x));
        (*lineItems)[i]->setPen(pen);
        scene->addItem((*lineItems)[i]);
    }

    // Then draw the driving route
    pen.setWidth(2);
    pen.setColor(QColor(163,73,164));
    Point_uint prev=route[0];
    for(size_t i=1,j=tester.getLineSize();i<route.size();i++){
        (*lineItems)[j+i]=(new QGraphicsLineItem(prev.y,prev.x,route[i].y,route[i].x));
        (*lineItems)[j+i]->setPen(pen);
        scene->addItem((*lineItems)[j+i]);
        prev=route[i];
    }

    ui->actionSearch->setEnabled(true);
    ui->actionDrive->setEnabled(true);
}









/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia. For licensing terms and
** conditions see http://qt.digia.com/licensing. For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights. These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "renderarea.h"
#include "ui_mainwindow.h"
#include "console.h"
#include "settingsdialog.h"

#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QString.h>

//! [0]
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//! [0]
    ui->setupUi(this);
    renderArea = new RenderArea(centerX,centerY);
    //setCentralWidget(renderArea);
    //addDockWidget(renderArea);
    console = new Console;
    console->setEnabled(false);
    //addDockWidget(console);
    //setCentralWidget(console);
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(renderArea);
    mainLayout->addWidget(console);

    //Set the ratio of drawing and console area
    QSizePolicy spLeft(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spLeft.setHorizontalStretch(5);
    renderArea->setSizePolicy(spLeft);
    QSizePolicy spRight(QSizePolicy::Preferred, QSizePolicy::Preferred);
    spRight.setHorizontalStretch(2);
    console->setSizePolicy(spRight);

    QWidget *central = new QWidget(this); // a central widget
    central->setLayout(mainLayout);
    setCentralWidget(central); // you were missing this

//! [1]
    serial = new QSerialPort(this);
//! [1]
    settings = new SettingsDialog;

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    initActionsConnections();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

//! [2]
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
//! [2]
    connect(console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));
//! [3]
}
//! [3]

MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}

//! [4]
void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    lidarRange=(float)p.lidarRange;
    showBadPoints=p.showBadPoints;
    if (serial->open(QIODevice::ReadWrite)) {
            console->setEnabled(true);
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            ui->actionConfigure->setEnabled(false);
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Open error"));
    }
}
//! [4]

//! [5]
void MainWindow::closeSerialPort()
{
    serial->close();
    console->setEnabled(false);
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    ui->statusBar->showMessage(tr("Disconnected"));
}
//! [5]

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Simple Terminal"),
                       tr("The <b>Simple Terminal</b> example demonstrates how to "
                          "use the Qt Serial Port module in modern GUI applications "
                          "using Qt, with a menu bar, toolbars, and a status bar."));
}

//! [6]
void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}
//! [6]

//! [7]
void MainWindow::readData()
{
    //Read the buffer in serial port
    QByteArray data = serial->readAll();

    /*
     * The size of a data packet in XV11 is 22.Since the data read from
     * serial port buffer vary in length so we need a pointer to record
     * the current point,which is why static 'i' and buffer is used here.
     * */
    static unsigned int i=0;
    static unsigned int tempBuffer[22];
    int correctData=0;
    static int packetReceived=0;

    /*
     * The fomart of a packet:
     * <start> <index> <speed_L> <speed_H> [Data 0] [Data 1] [Data 2] [Data 3] <checksum_L> <checksum_H>
     * where:
     * start = 0xFA
     * index = [0xA0,0xF9](means packet 0 to 89 with 4 data in one packet)
     * speed = little endian
     * [Data x] = 4 bytes long data,each means:
     *   byte:  |    0    |        1        |     2    |    3
     *   bit :  |  7 - 0  |   7     6   5-0 |   7 - 0  |  7 - 0
     *          |   dis   |invalid,s.w.,dis |  s.s LSB |  s.s MSB
     *   s.w: signal warning
     *   s.s: signal strength
     * */
    int k=0;
    for(k=0;k<data.size();k++)
    {
        if((unsigned char)(data.at(k))==0xFA)
        {
            if(i==0)//Receive a whole packet,process it
            {
                if(++packetReceived==89)
                {
                    //Todo: The number is hopping,could be wrong
                    packetReceived=0;
                    for(int j=0;j<360;j++)
                    {
                        correctData+=LidarBuffer[j].valid?1:0;
                    }
                    ui->statusBar->showMessage("Valid packet rate: "+QString::number(correctData)+"/360");
                    correctData=0;

                }

                unsigned int temp1,temp2;
                unsigned int distance;
                int index=tempBuffer[1]-0xA0;
                if(index<0||index>89)
                {
                    //Error,discard this packet and start over
                    i=1;
                    tempBuffer[0]=0xFA;
                    continue;
                }
                index*=4;



                //Verify checksum,if error,no need to do the rest
                int tempCheck[10];
                int n=0;
                for(n=0;n<10;n++)
                {
                    tempCheck[n]=tempBuffer[n*2]|(tempBuffer[n*2+1]<<8);
                }

                int check32=0;
                int result=0;

                for(n=0;n<10;n++)
                {
                    check32=(check32<<1)+tempCheck[n];
                }

                result=(check32&0x7FFF)+(check32>>15);
                result=result&0x7FFF;

                if(result)
                {
                    //Todo :what does the result mean?
                }

                /*
                 * Convert the speed.The lowest bits are decimal,thus divide by 64.
                 * Since there are 4 points in one packet so we just simply set them
                 * to the same speed.
                 *
                 * */
                temp1=tempBuffer[2];
                temp2=tempBuffer[3];
                temp1=(temp2<<8)|temp1;
                LidarBuffer[index].speed=temp1/64.0;
                LidarBuffer[index+1].speed=temp1/64.0;
                LidarBuffer[index+2].speed=temp1/64.0;
                LidarBuffer[index+3].speed=temp1/64.0;

                //Interpret 4 point data one by one,each point consists 4 bytes
                for(int j=0,pos=4;j<4;j++,pos+=4)//starts from byte 4,8,12,16
                {
                    //Interpret byte 0 and byte 1,which holds range information
                    temp1=tempBuffer[pos];
                    temp2=tempBuffer[pos+1];

                    //Convert range data,only 14 bits valid
                    distance=(temp1|(temp2<<8));
                    distance=distance&0x3FFF;
                    LidarBuffer[index+j].distance=distance;//14 bits

                    //Check if the data is valid
                    if(((temp2>>7)&1)==1)
                    {
                        LidarBuffer[index+j].valid=false;
                        //Should break
                        LidarBuffer[index+j].errorCode=temp1;
                    }
                    else
                    {
                        LidarBuffer[index+j].valid=true;
                        LidarBuffer[index+j].errorCode=0;
                    }

                    //Interpret byte 2 and byte 3,which holds signal strength
                    temp1=tempBuffer[pos+2];
                    temp2=tempBuffer[pos+3];

                    LidarBuffer[index+j].strength=temp1|(temp2<<8);
                }

                /*
                 * Packet interpretation success,let's test it
                 * Format the data:
                 * 0 - 360   : speed /rpm | point1  point2  point3  point4
                 * If the packet is broken then set the points to 'x'
                 * */
                QByteArray test;
                test.append(QString::number(index));
                test.append("\t:");
                test.append(QString::number((int)LidarBuffer[index].speed));
                for(int m=0;m<4;m++)
                {
                    test.append("\t");
                    if(LidarBuffer[index].valid)
                    {
                        test.append(QString::number(LidarBuffer[index+m].distance));
                        //correctData+=1;//count the number of correct data
                    }
                    else
                        test.append("x");
                }
                test.append("\n");
                console->putData(test);


                //Now draw the points using range and angle data
                for(int m=0;m<4;m++)
                {
                    QPoint p;

                    if(LidarBuffer[index+m].valid)
                    {
                        p.setX(centerX+qCos((index+m)/360.0*2*M_PI)*LidarBuffer[index+m].distance/lidarRange);
                        p.setY(centerX+qSin((index+m)/360.0*2*M_PI)*LidarBuffer[index+m].distance/lidarRange);
                    }
                    else
                    {
                        /*
                         * For some reason my LIDAR has lots of bad points,which are some points
                         * gathering around closely to the center.So this option filter out these
                         * bad points because the LIDAR can't not detect anything within 15 cm.
                         * */
                        if(showBadPoints)
                        {
                            p.setX(centerX+qCos((index+m)/360.0*2*M_PI)*LidarBuffer[index+m].distance/lidarRange);
                            p.setY(centerX+qSin((index+m)/360.0*2*M_PI)*LidarBuffer[index+m].distance/lidarRange);
                        }
                        else
                        {
                            p.setX(centerX);
                            p.setY(centerX);
                        }

                    }
                    points[index+m]=p;
                }
                if(index==0)
                {
                    renderArea->setPoint(points);
                }

            }

            //If the 0xFA starts in the middle of 22,start over
            i=1;
            tempBuffer[0]=0xFA;
        }
        else//Put data in tempBuffer
        {
            tempBuffer[i++]=(unsigned char)data.at(k);
            i%=22;
        }
    }



}
//! [7]

//! [8]
void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}
//! [8]

void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
    connect(ui->actionClear, SIGNAL(triggered()), console, SLOT(clear()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

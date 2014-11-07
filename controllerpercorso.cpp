#include "controllerpercorso.h"
#include "ui_controllerpercorso.h"
#include <qlabel.h>
#include <QtWidgets>
#include <block.h>
#include "settingsdialog.h"
#include <QFile>


#include<QtSerialPort/QSerialPort>
ControllerPercorso::ControllerPercorso(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ControllerPercorso)
{
    ui->setupUi(this);
    setupPercorso();
//! [1]
    serial = new QSerialPort(this);
//! [1]
    settings = new SettingsDialog;

    ui->actionConnetti->setEnabled(true);
    ui->actionDisconnetti->setEnabled(false);
   // ui->actionQuit->setEnabled(true);
    ui->actionConfigura->setEnabled(true);

    initActionsConnections();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

//! [2]
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
//! [2]
  //  connect(console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));
//! [3]




}
QList<Block*> ControllerPercorso::Percorso;

ControllerPercorso::~ControllerPercorso()
{
    delete ui;

}

void ControllerPercorso::avanti()
{
    listaBlocchi.push_back(new Block(Avanti,this));
}

void ControllerPercorso::destra()
{
    listaBlocchi.push_back(new Block(Dx,this));
}

void ControllerPercorso::sinistra()
{
    listaBlocchi.push_back(new Block(Sx,this));
}

void ControllerPercorso::loop()
{
    listaBlocchi.push_back(new Block(Loop,this));
    ui->actionLoop->setEnabled(false);
}

void ControllerPercorso::continua()
{
    listaBlocchi.push_back(new Block(Continue1,this));
    listaBlocchi.push_back(new Block(Continue2,this));
}

void ControllerPercorso::invia()
{
     QByteArray * percorso;
     int speed;
     unsigned int i;
     percorso = new QByteArray();
     //speed = calcolaspeed();
     for (i =0;i<ControllerPercorso::Percorso.size();i++){
         if(ControllerPercorso::Percorso[i]->type  == Start)percorso->push_back(0x24);
         else if(ControllerPercorso::Percorso[i]->type== Loop){
            percorso->push_back(0x2);
            break;
         }
         else if(ControllerPercorso::Percorso[i]->type== End){
            percorso->push_back(0xff);
            break;
         }
         else if(ControllerPercorso::Percorso[i]->type== Avanti){
            percorso->push_back(0x0f);
            percorso->push_back(0x19);
         }
         else if(ControllerPercorso::Percorso[i]->type== Sx){
            percorso->push_back(0x80);
            percorso->push_back(0x1F);
         }
         else if(ControllerPercorso::Percorso[i]->type== Dx){
            percorso->push_back(0x40);
            percorso->push_back(0x1F);
         }
     }

    writeData(*percorso);
    QMessageBox::information(this,"Successo","Percorso inviato con successo");
}

void ControllerPercorso::openSerialPort()
{

        SettingsDialog::Settings p = settings->settings();
        serial->setPortName(p.name);
        serial->setBaudRate(p.baudRate);
        serial->setDataBits(p.dataBits);
        serial->setParity(p.parity);
        serial->setStopBits(p.stopBits);
        serial->setFlowControl(p.flowControl);
        if (serial->open(QIODevice::ReadWrite)) {
                //console->setEnabled(true);
                //console->setLocalEchoEnabled(p.localEchoEnabled);
                ui->actionConnetti->setEnabled(false);
                ui->actionDisconnetti->setEnabled(true);
                ui->actionConfigura->setEnabled(false);
                ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6").arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits).arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
        } else {
            QMessageBox::critical(this, tr("Error"), serial->errorString());

            ui->statusBar->showMessage(tr("Open error"));
            if(serial->isOpen())
            QMessageBox::information(this,"boh","porta abile");
        }

}

void ControllerPercorso::closeSerialPort()
{
    serial->close();
    //console->setEnabled(false);
    ui->actionConnetti->setEnabled(true);
    ui->actionDisconnetti->setEnabled(false);
    ui->actionConfigura->setEnabled(true);
    ui->statusBar->showMessage(tr("Disconnected"));
}

void ControllerPercorso::about()
{
    QMessageBox::about(this, tr("About Simple Terminal"),
                       tr("The <b>Simple Terminal</b> example demonstrates how to "
                          "use the Qt Serial Port module in modern GUI applications "
                          "using Qt, with a menu bar, toolbars, and a status bar."));
}

void ControllerPercorso::writeData(const QByteArray &data)
{
    serial->write(data);
}

void ControllerPercorso::readData()
{
    QByteArray data = serial->readAll();
    // console->putData(data);
}

void ControllerPercorso::salva()
{
    int i;
    QString fileName = QFileDialog::getSaveFileName(this,
             tr("Save percorso"), "",
             tr("Percorso (*.pcr);;All Files (*)"));
    if (fileName.isEmpty())
           return;
       else {
           QFile file(fileName);
           if (!file.open(QIODevice::WriteOnly)) {
               QMessageBox::information(this, tr("Unable to open file"),
                   file.errorString());
               return;
           }

           QTextStream out(&file);

           for (i =0;i<ControllerPercorso::Percorso.size();i++){
               if(ControllerPercorso::Percorso[i]->type  == Start)
                   out<<"Start"<<endl;
               else if(ControllerPercorso::Percorso[i]->type== Loop){
                  out<<"Loop"<<endl;
               }
               else if(ControllerPercorso::Percorso[i]->type== End){
                  out<<"End"<<endl;
               }
               else if(ControllerPercorso::Percorso[i]->type== Avanti){
                  out<<"Avanti"<<endl;
               }
               else if(ControllerPercorso::Percorso[i]->type== Sx){
                  out<<"Sx"<<endl;
               }
               else if(ControllerPercorso::Percorso[i]->type== Dx){
                 out<<"Dx"<<endl;
               }
            }
           file.close();
    }
}

void ControllerPercorso::apri()
{
    QList<Block*> Percorso_tmp;
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Apri Percorso"), "",
            tr("Address Book (*.pcr);;All Files (*)"));
    if (fileName.isEmpty())
            return;
        else {

            QFile file(fileName);

            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;
            }

            QTextStream in(&file);
            //in.setVersion(QDataStream::Qt_4_5);
            //contacts.empty();   // empty existing contacts
            //in >> Percorso_tmp;
    file.close();
    }
}

void ControllerPercorso::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Criticall Error"), serial->errorString());
        closeSerialPort();
    }
}

void ControllerPercorso::setupPercorso()
{
    listaBlocchi.clear();
    listaBlocchi.push_back(new Block(Start,this));
    listaBlocchi.push_back(new Block(End,this));
    Percorso.push_back(listaBlocchi.first());

    connect(ui->actionAvanti,SIGNAL(triggered()),this,SLOT(avanti()));
    connect(ui->actionSvolta_Destra,SIGNAL(triggered()),this,SLOT(destra()));
    connect(ui->actionSvolta_Sinistra,SIGNAL(triggered()),this,SLOT(sinistra()));
    connect(ui->actionLoop,SIGNAL(triggered()),this,SLOT(loop()));
    connect(ui->actionContinua,SIGNAL(triggered()),this,SLOT(continua()));
    connect(ui->actionInvia_Percorso,SIGNAL(triggered()),this,SLOT(invia()));
    connect(ui->actionSalva,SIGNAL(triggered()),this,SLOT(salva()));
    connect(ui->actionApri,SIGNAL(triggered()),this,SLOT(apri()));
}

void ControllerPercorso::initActionsConnections()
{
    connect(ui->actionConnetti, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnetti, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionConfigura, SIGNAL(triggered()), settings, SLOT(show()));
/*    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->actionClear, SIGNAL(triggered()), console, SLOT(clear()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));*/
}

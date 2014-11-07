#ifndef CONTROLLERPERCORSO_H
#define CONTROLLERPERCORSO_H
#include <QtCore/QtGlobal>
#include <QMainWindow>
#include <block.h>
#include <QList>
#include<QByteArray>
#include <QtSerialPort/QSerialPort>
namespace Ui {
class ControllerPercorso;
}

//class Console;
class SettingsDialog;

class ControllerPercorso : public QMainWindow
{
    Q_OBJECT

public:
    explicit ControllerPercorso(QWidget *parent = 0);
    ~ControllerPercorso();
    static QList<Block *> Percorso;
private slots:
    void avanti();
    void destra();
    void sinistra();
    void loop();
    void continua();
    void invia();
    void openSerialPort();
    void closeSerialPort();
    void about();
    void writeData(const QByteArray &data);
    void readData();
    void salva();
    void apri();

    void handleError(QSerialPort::SerialPortError error);
private:
    Ui::ControllerPercorso *ui;
    void setupPercorso();
    void initActionsConnections();
    QList <Block *>  listaBlocchi;
   // Console *console;
    SettingsDialog * settings;
    QSerialPort *serial;
};

#endif // CONTROLLERPERCORSO_H

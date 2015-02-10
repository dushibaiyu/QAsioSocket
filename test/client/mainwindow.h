#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include "qasiotcpsocket.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void readError(QAsioTcpSocket::SocketErroSite & site,const asio::error_code & erro_code);

    void on_pushSent_clicked();
    void on_pushConnect_clicked();
    void on_timeBut_clicked();

private:
    Ui::MainWindow * ui;
    QAsioTcpSocket * socket;
    QTimer tm;
    QStringList list;
};

#endif // MAINWINDOW_H

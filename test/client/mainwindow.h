#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <qasiotcpsocket.h>

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
    void readError(const QString & erro);

    void on_pushSent_clicked();
    void on_pushConnect_clicked();
    void on_timeBut_clicked();

    void connectdd();
    void disconnectdd();
    void readData(const QByteArray & data);
private:
    Ui::MainWindow * ui;
    QAsioTcpsocket * socket;
    QTimer tm;
    QStringList list;
};

#endif // MAINWINDOW_H

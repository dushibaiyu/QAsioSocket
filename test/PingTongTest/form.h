#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include "sockets.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

protected slots:
    void on_pushButton_clicked();
    void haveData(int site ,int revCou, int sentCou,qint64 revSize,qint64 sentSize);

private:
    sockets socket;
    Ui::Form *ui;
};

#endif // FORM_H

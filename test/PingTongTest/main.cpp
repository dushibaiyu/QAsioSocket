#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include <QApplication>
#include "form.h"


int main(int argc, char *argv[])
{
//    qInstallMessageHandler(customMessageHandler);
    QApplication a(argc, argv);
    Form fm;
    fm.show();
    return a.exec();;
}

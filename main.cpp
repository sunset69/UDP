#include "controller.h"

#include <QApplication>
#include <QThread>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "The main threadID is :" << QThread::currentThreadId();
    Controller w;
    w.show();
    return a.exec();
}

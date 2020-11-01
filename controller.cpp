#include "controller.h"
#include "ui_controller.h"
#include <QThread>
#include <QDebug>
#include<QMessageBox>

Controller::Controller(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Controller)
{
    ui->setupUi(this);
    qDebug() << "Controller's thread is :" << QThread::currentThreadId();

    m_worker = new Worker();
    m_worker->moveToThread(&m_workThread);
//    m_worker->on_doSomething();

    connect(this, &Controller::startRunning, m_worker, &Worker::on_doSomething);
    connect(&m_workThread, &QThread::finished, m_worker, &QObject::deleteLater);
    connect(m_worker, &Worker::resultReady, this, &Controller::on_receivResult);
    m_workThread.start();
//    this->start();

    ui->te_recv->setReadOnly(true);
    connect(ui->btn_listen,&QPushButton::clicked,[=](){
        ui->lE_port->setReadOnly(true);
        int m_port = ui->lE_port->text().toInt();
        if( m_port != NULL)
        {
            qDebug()<<"NOT NULL!!!";
        m_worker->setPort(m_port);
        }else {
        ui->lE_port->setText("4567");
        }
    });

    connect(m_worker,&Worker::canSend,[=](){
        this->issend  =true;
    });

    connect(ui->btn_listen,SIGNAL(clicked()),this,SLOT(start()));
    connect(ui->btn_send,&QPushButton::clicked,[=](){
        QString response = ui->te_send->toPlainText();
//        qDebug()<<response;
        if(issend){
        m_worker->setResponse(response);
        qDebug()<<"control::"<<response;
        ui->te_send->clear();
        issend = false;
        }else {
        QMessageBox::information(this,"warnning","等待客户端连接！",QMessageBox::Ok);
        }
    });

}

Controller::~Controller()
{
    m_workThread.quit();
    m_workThread.wait();
    delete ui;
}

void Controller::start()
{
//    qDebug() <<"发送了信号startRunning()\n";
  emit startRunning();
}

void Controller::on_receivResult(const QString &str)
{
  qDebug() << str;
  ui->te_recv->append(str);
}

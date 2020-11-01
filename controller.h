#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QWidget>
#include <QThread>
#include "worker.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Controller; }
QT_END_NAMESPACE

class Controller : public QWidget
{
    Q_OBJECT

public:
    Controller(QWidget *parent = nullptr);
    ~Controller();


private:
    Ui::Controller *ui;

public: signals:
  void startRunning(); // 用于触发新线程中的耗时操作函数
  void responseContent(const QString &str); // 向外界发送结果

public slots:
  void on_receivResult(const QString &str); // 接收新线程中的结果
    void start();

private:
  QThread m_workThread;
  Worker *m_worker;
  bool issend = false;
};
#endif // CONTROLLER_H

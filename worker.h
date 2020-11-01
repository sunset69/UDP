#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <Winsock2.h>
#pragma comment(lib,"WS2_32.lib")
#define BUFFER_SIZE 1024
//#define PORT 233


class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);

    void setPort(int port);
//    SOCKADDR_IN creatIP(int port = 233,long ip = INADDR_ANY);

private:
    WSADATA WSAData;
    char receBuf[BUFFER_SIZE];
    char Response[BUFFER_SIZE] = " ";
//    SOCKADDR_IN addr_Server; //服务器的地址等信息
    SOCKADDR_IN addr_Clt;
    int fromlen = sizeof(SOCKADDR);
    SOCKET sockServer;
    int PORT = 4567;
    bool isSend = false;

signals:
  void resultReady(const QString &str); // 向外界发送结果
  void canSend();

public slots:
  void on_doSomething(); // 耗时操作
  void setResponse(const QString &str);
};

#endif // WORKER_H

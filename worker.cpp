#include "worker.h"
#include <QDebug>
#include <QThread>

Worker::Worker(QObject *parent) : QObject(parent)
{

}
void Worker::on_doSomething()
{
  qDebug() << "I'm working in thread:" << QThread::currentThreadId();
  emit resultReady("Hello");

    //判断协议栈是否正确安
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
    {
//        QMessageBox::information(this,"error","初始化失败！",QMessageBox::Yes);
        emit resultReady("初始化失败！");
        exit(1);
    }

    //服务器端套接口
   sockServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockServer == INVALID_SOCKET)
    {
//        QMessageBox::information(this,"error","Failed socket() \n",QMessageBox::Yes);
        emit resultReady("Failed socket() ");
    }

//    addr_Server = creatIP( );
    SOCKADDR_IN addr_Server; //服务器的地址等信息
    addr_Server.sin_family = AF_INET;
//    addr_Server.sin_port = htons(4567);
    addr_Server.sin_port = htons(PORT);
    qDebug()<<"port:"<<PORT;
    addr_Server.sin_addr.S_un.S_addr = INADDR_ANY;

    if (bind(sockServer, (SOCKADDR*)&addr_Server, sizeof(addr_Server)) == SOCKET_ERROR)
    {
        //服务器与本地地址绑定
//        QMessageBox::information(this,"error", QString("Failed socket(): %1 \n").arg( WSAGetLastError()),QMessageBox::Yes);
        emit resultReady(QString("Failed socket(): %1 \n").arg( WSAGetLastError()));
    }
    emit resultReady("waitting client... ");

    while(true){
    int last = recvfrom(sockServer, receBuf, 1024, 0, (SOCKADDR*)&addr_Clt, &fromlen);
    if (last > 0)
    {
        //判断接收到的数据是否为空
        receBuf[last] = '\0';//给字符数组加一个'\0'，表示结束了。不然输出有乱码
        if (strcmp(receBuf, "bye") == 0)
        {
//            cout << " 客户端不跟我聊天了..." << endl;
            emit resultReady(" 客户端不跟我聊天了... ");
            emit resultReady("closing...");
            closesocket(sockServer);
        }
        else
        {
//            printf("接收到数据（%s）：%s\n", inet_ntoa(addr_Clt.sin_addr), receBuf);
            emit resultReady(QString("接收到数据（%1）：%2").arg(inet_ntoa(addr_Clt.sin_addr)).arg(receBuf));

            //可以发送
            emit canSend();
        }
    }
    //    std::cout << "回复客户端消息!";
        emit resultReady("回复客户端消息!");
    while (!isSend) {

    }
    isSend = false;
    emit resultReady(QString("发送数据：%1").arg(Response));
    sendto(sockServer, Response, strlen(Response), 0, (SOCKADDR*)&addr_Clt, sizeof(SOCKADDR));
    if (strcmp(Response, "bye") == 0)
    {
//        cout << "closing...";
        emit resultReady("closing...");
        break;
    }
    }
    closesocket(sockServer);

    WSACleanup();
}
void  Worker::setPort(int port)
{
    PORT = port;
}
//SOCKADDR_IN Worker::creatIP(int port, long ip)
//{
//    SOCKADDR_IN ipadd; //服务器的地址等信息
//    ipadd.sin_family = AF_INET;
//    ipadd.sin_port = htons(port);
//    ipadd.sin_addr.S_un.S_addr = ip;
//    qDebug()<<"ip:"<<ip<<" port:"<<port;
//}
void Worker::setResponse(const QString &str)
{
//    qDebug()<<"str:"<<str;
//    QByteArray ba = str.toUtf8();
//    Response = ba.data();
    QString s = str;
//    sscanf(Response,"%s",&s);
    strcpy(Response,str.toUtf8());
    qDebug()<<"worker:"<<Response;
    isSend = true;
}

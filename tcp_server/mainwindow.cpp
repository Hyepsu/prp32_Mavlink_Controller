#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = new QTcpServer();

    //连接信号槽
    connect(server,&QTcpServer::newConnection,this,&MainWindow::server_New_Connect);
}

MainWindow::~MainWindow()
{
    server->close();
    server->deleteLater();
    delete ui;
}

void MainWindow::on_pushButton_Listen_clicked()
{
    if(ui->pushButton_Listen->text() == tr("Listen"))
    {
        //监听指定的端口
        if(!server->listen(QHostAddress::Any, 8989))
        {
            //若出错，则输出错误信息
            //qDebug()<<server->errorString();
            ui->textBrowser->append(server->errorString());
            return;
        }
        //修改按键文字
        ui->pushButton_Listen->setText("Cancel");
        //qDebug()<< "Listen succeessfully!";
        ui->textBrowser->append("Listen succeessfully!");
    }
    else
    {
        //如果正在连接，点击侦听后立即取消侦听
        if(socket->state() == QAbstractSocket::ConnectedState)
        {
            //关闭连接
            socket->disconnectFromHost();
        }
        //取消侦听
        server->close();
        //修改按键文字
        ui->pushButton_Listen->setText("Listen");
    }

}

void MainWindow::server_New_Connect()
{
    //获取客户端连接
    socket = server->nextPendingConnection();

    //连接QTcpSocket的信号槽，以读取新数据
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);

    //qDebug() << "A Client connect!";
    ui->textBrowser->append("A Client connect!");
}

void MainWindow::socket_Read_Data()
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer=socket->readAll();
    if (!buffer.isEmpty())
    {
        //刷新显示
        QString str;
        str+=tr(buffer.toHex());
        ui->textBrowser->append("Received data(Hexadecimal):");
        ui->textBrowser->append(str);
        //qDebug()<<buffer.toHex();
    }
}

void MainWindow::socket_Disconnected()
{
    //qDebug() << "Disconnected!";
    ui->textBrowser->append("Disconnected!");
}

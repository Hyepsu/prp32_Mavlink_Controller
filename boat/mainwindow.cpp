#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <mavlink.h>
#include <QWidget>
#include <iostream>

uint8_t boat_sysid=0xff;
uint8_t boat_compid=0xbe;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket();

    //连接信号槽
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socket_Read_Data);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_Disconnected);

    ui->pushButton_Forward->setEnabled(false);
    ui->pushButton_Backward->setEnabled(false);
    ui->pushButton_Speedup->setEnabled(false);
    ui->pushButton_Speeddown->setEnabled(false);
    ui->pushButton_Left->setEnabled(false);
    ui->pushButton_Right->setEnabled(false);
    ui->pushButton_STOP->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete this->socket;
    delete ui;
}

void MainWindow::on_pushButton_Connect_clicked()
{
    if(ui->pushButton_Connect->text() == tr("Connect"))
    {
        //取消已有的连接
        socket->abort();


        //连接服务器
        socket->connectToHost("127.0.0.1", 8989);

       //等待连接成功.
        if(!socket->waitForConnected(30000))
       {
           //qDebug() << "Connection failed!";
           ui->textBrowser->append("Connection failed!");
           return;
       }
       //qDebug() << "Connect successfully!";
       ui->textBrowser->append("Connect successfully!");

       //指令按键使能
       ui->pushButton_Forward->setEnabled(true);
       ui->pushButton_Backward->setEnabled(true);
       ui->pushButton_Speedup->setEnabled(true);
       ui->pushButton_Speeddown->setEnabled(true);
       ui->pushButton_Left->setEnabled(true);
       ui->pushButton_Right->setEnabled(true);
       ui->pushButton_STOP->setEnabled(true);

       //修改按键文字
       ui->pushButton_Connect->setText("Disconnect");
   }
   else
   {
       //断开连接
        socket->disconnectFromHost();

       //修改按键文字
        ui->pushButton_Connect->setText("Connect");

       //指令按键失能
       ui->pushButton_Forward->setEnabled(false);
       ui->pushButton_Backward->setEnabled(false);
       ui->pushButton_Speedup->setEnabled(false);
       ui->pushButton_Speeddown->setEnabled(false);
       ui->pushButton_Left->setEnabled(false);
       ui->pushButton_Right->setEnabled(false);
       ui->pushButton_STOP->setEnabled(false);
   }
}

void MainWindow::on_pushButton_Forward_clicked()
{
    uint8_t control_state[16]={0};//对应PAYLOAD部分
    uint8_t buffer[24]={0};       //对应Mavlink消息帧
    mavlink_message_t  mavlink_tx_msg;
    control_state[0]=0x01;       //前进控制指令
    mavlink_msg_contral_ship_mode_pack(boat_sysid,boat_compid,&mavlink_tx_msg,control_state);//打包函数
    uint16_t len = mavlink_msg_to_send_buffer(buffer, &mavlink_tx_msg); //完成Mavlink消息帧的封装
    //qDebug()<<mavlink_tx_msg.magic;
    socket->write((char *)buffer,len);   //套接字写入数据
    socket->flush();           //清空缓存使消息发送至服务器端
    ui->textBrowser->append("Forward instruction sent out!");
}

void MainWindow::on_pushButton_Backward_clicked()
{
    uint8_t control_state[16]={0};
    uint8_t buffer[24]={0};
    mavlink_message_t  mavlink_tx_msg;
    control_state[0]=0x02;
    mavlink_msg_contral_ship_mode_pack(boat_sysid,boat_compid,&mavlink_tx_msg,control_state);
    uint16_t len = mavlink_msg_to_send_buffer(buffer, &mavlink_tx_msg);
    /*
    for(int i=0;i<24;i++)
    {
        qDebug("%x",(unsigned char)(buffer[i]));
    }
    */
    socket->write((char *)buffer,len);
    qDebug()<<mavlink_tx_msg.magic;
    socket->flush();
    ui->textBrowser->append("Backward instruction sent out!");
}

void MainWindow::on_pushButton_Speedup_clicked()
{
    uint8_t control_state[16]={0};
    uint8_t buffer[24]={0};
    mavlink_message_t  mavlink_tx_msg;
    control_state[2]=0x01;
    mavlink_msg_contral_ship_mode_pack(boat_sysid,boat_compid,&mavlink_tx_msg,control_state);
    uint16_t len = mavlink_msg_to_send_buffer(buffer, &mavlink_tx_msg);
    qDebug()<<mavlink_tx_msg.magic;
    socket->write((char *)buffer,len);
    socket->flush();
    ui->textBrowser->append("Speedup instruction sent out!");
}

void MainWindow::on_pushButton_Speeddown_clicked()
{
    uint8_t control_state[16]={0};
    uint8_t buffer[24]={0};
    mavlink_message_t  mavlink_tx_msg;
    control_state[2]=0x02;
    mavlink_msg_contral_ship_mode_pack(boat_sysid,boat_compid,&mavlink_tx_msg,control_state);
    uint16_t len = mavlink_msg_to_send_buffer(buffer, &mavlink_tx_msg);
    qDebug()<<mavlink_tx_msg.magic;
    socket->write((char *)buffer,len);
    socket->flush();
    ui->textBrowser->append("Speeddown instruction sent out!");
}

void MainWindow::on_pushButton_Left_clicked()
{
    uint8_t control_state[16]={0};
    uint8_t buffer[24]={0};
    mavlink_message_t  mavlink_tx_msg;
    control_state[1]=0x01;
    mavlink_msg_contral_ship_mode_pack(boat_sysid,boat_compid,&mavlink_tx_msg,control_state);
    uint16_t len = mavlink_msg_to_send_buffer(buffer, &mavlink_tx_msg);
    socket->write((char *)buffer,len);
    socket->flush();
    ui->textBrowser->append("Left-turn instruction sent out!");
}

void MainWindow::on_pushButton_Right_clicked()
{
    uint8_t control_state[16]={0};
    uint8_t buffer[24]={0};
    mavlink_message_t  mavlink_tx_msg;
    control_state[1]=0x02;
    mavlink_msg_contral_ship_mode_pack(boat_sysid,boat_compid,&mavlink_tx_msg,control_state);
    uint16_t len = mavlink_msg_to_send_buffer(buffer, &mavlink_tx_msg);
    socket->write((char *)buffer,len);
    socket->flush();
    ui->textBrowser->append("Right-turn instruction sent out!");
}

void MainWindow::on_pushButton_STOP_clicked()
{
    uint8_t control_state[16]={0};
    uint8_t buffer[24]={0};
    mavlink_message_t  mavlink_tx_msg;
    control_state[0]=0x03;
    mavlink_msg_contral_ship_mode_pack(boat_sysid,boat_compid,&mavlink_tx_msg,control_state);
    uint16_t len = mavlink_msg_to_send_buffer(buffer, &mavlink_tx_msg);
    socket->write((char *)buffer,len);
    socket->flush();
    ui->textBrowser->append("Stop instruction sent out!");
}

void MainWindow::socket_Read_Data()
{

}

void MainWindow::socket_Disconnected()
{
    //指令按键失能
    ui->pushButton_Forward->setEnabled(false);
    ui->pushButton_Backward->setEnabled(false);
    ui->pushButton_Speedup->setEnabled(false);
    ui->pushButton_Speeddown->setEnabled(false);
    ui->pushButton_Left->setEnabled(false);
    ui->pushButton_Right->setEnabled(false);
    ui->pushButton_STOP->setEnabled(false);

    //修改按键文字
    ui->pushButton_Connect->setText("Connect");
    ui->textBrowser->append("Disconnected!");
    //qDebug() << "Disconnected!";
}

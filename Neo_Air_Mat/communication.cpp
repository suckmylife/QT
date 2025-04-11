#include "communication.h"
QMutex communication::mutex;
QSerialPort* communication::serialPort = nullptr;

communication::communication(QObject *parent)
    : QObject(parent), _status(false)
{
    serialPort = communication::getInstance();
}
communication::~communication()
{
    if(serialPort->isOpen())
    {
        serialPort->close();
    }
    delete serialPort;
}

void communication::send_cmd(QString cmd)
{
    QMutexLocker locker(&mutex);
    QByteArray request;
    request = QByteArray::fromHex(cmd.toUtf8());
    serialPort->write(request);
    if(serialPort->waitForBytesWritten(10))
    {
        return;
    }
}

QSerialPort* communication::getInstance() {
    if (!serialPort) {
        serialPort = new QSerialPort();
        // 여기에서 serialPort의 초기 설정을 할 수 있습니다.
        serialPort->setPortName("ttyAMA1");
        //serialPort->setPortName("ttyS0");
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setBaudRate((QSerialPort::Baud115200));
    }
    return serialPort;
}

void communication::releaseInstance() {
    if (serialPort) {
        delete serialPort;
        serialPort = nullptr;
    }
}

void communication::connect_serial()
{
    if(serialPort)
    {
        if(serialPort->isOpen()) // 포트가 이미 열려 있는 경우
        {
            serialPort->close(); // 기존 연결을 닫고
        }

        if(serialPort->open(QIODevice::ReadWrite)) // 다시 열기 시도
        {
            qDebug() << "Serial port opened successfully";
        }
        else
        {
            QMessageBox::critical(nullptr, QString::fromUtf8("연결상태"), QString::fromUtf8("실패"));
        }
    }
    else
    {
        qDebug() << "No Serial";
    }
}

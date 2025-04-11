#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <QSerialPort>
#include <QObject>
#include <QDebug>
#include <QMessageBox>
#include <QMutex>

class communication : public QObject {
    Q_OBJECT

public:
    static communication* instance()
    {
        static communication instance;
        return &instance;
    }
    virtual ~communication();
    static QSerialPort* getInstance();
    static void releaseInstance();
    static QSerialPort* serialPort;
    void send_cmd(QString cmd);

private:
    explicit communication(QObject *parent = nullptr);

    communication(const communication&) = delete;
    communication& operator=(const communication&) = delete;
    static QMutex mutex;

public slots:
    void connect_serial();

signals:
    void requestReset(const QByteArray &reset);
    void request_finish();

private:
    bool _status;

    QVector<float> result;
    QVector<QString> requests;
    time_t timer;
    struct tm* t;

};

#endif // COMMUNICATION_H

#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QObject>
#include <QVariant>
#include <QMessageBox>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(const QString &dbName, QObject *parent = nullptr);
    ~Database();

    bool open();
    void close();
    int insert_patient_info(const QString &birth,
                            const QString &kg,
                            const QString &height,
                            const QString &gender);

    bool update_patient_info(const QString &birth,
                             const QString &kg,
                             const QString &height,
                             const QString &gender,
                             int id);
    void set_Pressure(int id, QMap<QString, QString> datas);
    void get_mode(QString mode_);
    bool insert_serial_data();

private:
    QString databaseName;
    QString mode;
    QSqlDatabase db;
    int PaitentID;
    QMap<QString, QString> datas;

    QString mapToString(const QMap<QString,QString>& map);
};

#endif // DATABASE_H

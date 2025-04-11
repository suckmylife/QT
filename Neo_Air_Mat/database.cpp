#include "database.h"

Database::Database(const QString &dbName, QObject *parent)
        :QObject(parent), databaseName(dbName)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databaseName);
    db.open();
}

Database::~Database()
{
    close();
}

QString Database::mapToString(const QMap<QString, QString> &map)
{
    QJsonObject j_map;
    for(auto i = map.begin(); i != map.end(); ++i)
    {
        QString pressure = i.value();
        QString p = pressure.mid(12,16);
        QJsonArray p_data;
        for (int j = 0; j < pressure.size(); j += 4)
        {
            QString hexValue = pressure.mid(j, 4);
            p_data.append(QString::number(static_cast<float>(hexValue.toInt(nullptr, 16)) / 10));
        }

        j_map.insert(i.key(), p_data);
    }
    QJsonDocument j_doc(j_map);
    return QString(j_doc.toJson(QJsonDocument::Compact));
}

void Database::set_Pressure(int id, QMap<QString, QString> datas)
{
    this->PaitentID = id;
    this->datas     = datas;
}

void Database::get_mode(QString mode_)
{
    this->mode = mode_;
}

bool Database::insert_serial_data()
{
    if(datas.size()>27)
    {
        QSqlQuery query;
        QString j_map = mapToString(datas);
        QDateTime current_time = QDateTime::currentDateTime();
        QString dates = current_time.toString("yyyy-MM-dd HH:mm:ss");
        query.prepare("INSERT INTO Pressure(PatientID, raw_data, date, mode) VALUES(:PatientID, :raw_data, :date, :mode)");
        query.bindValue(":PatientID",PaitentID);
        query.bindValue(":raw_data",j_map);
        query.bindValue(":date",dates);
        query.bindValue(":mode", mode);
        if(!query.exec())
        {
            qDebug() << query.lastError().text();
            return false;
        }
    }
    return true;
}

bool Database::update_patient_info(const QString &birth, const QString &kg, const QString &height, const QString &gender, int id)
{
    QSqlQuery query;
    query.prepare("UPDATE PatientInfo SET birth = :birth, kg = :kg, height = :height, gender = :gender WHERE PatientID = :id");
    query.bindValue(":birth",birth.toInt());
    query.bindValue(":kg",kg.toInt());
    query.bindValue(":height",height.toInt());
    query.bindValue(":gender", gender);
    query.bindValue(":id", id);
    if(!query.exec())
    {
        qDebug() << query.lastError().text();
        return false;
    }
    return true;
}

int Database::insert_patient_info(const QString &birth, const QString &kg, const QString &height, const QString &gender)
{
    QSqlQuery query;
    query.prepare("INSERT INTO PatientInfo(birth, kg, height, gender) VALUES(:birth, :kg, :height, :gender)");
    query.bindValue(":birth",birth.toInt());
    query.bindValue(":kg",kg.toFloat());
    query.bindValue(":height",height.toFloat());
    query.bindValue(":gender", gender);

    if(!query.exec())
    {
        QMessageBox::critical(nullptr, QString::fromUtf8("데이터베이스 오류"), QString::fromUtf8("저장할 수 없습니다"));
        qDebug() << query.lastError().text();
        return -1;
    }
    return query.lastInsertId().toInt();
}

bool Database::open()
{
    if(!db.isOpen())
    {
        QMessageBox::critical(nullptr, QString::fromUtf8("데이터베이스 오류"), QString::fromUtf8("데이터베이스에 연결할 수 없습니다"));
        return false;
    }
    return true;
}

void Database::close()
{
    if(db.isOpen())
    {
        db.close();
    }
}

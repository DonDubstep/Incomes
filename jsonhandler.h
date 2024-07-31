#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QJsonObject>
#include <QFile>
#include <QCoreApplication>
#include <QMap>



typedef struct json_return
{
    int total;
    int parents;
    int moneybox;
    int total_last_month;
    QVector<QMap<QString, int>> map;
}json_return;

class JsonHandler
{
public:
    void writeJson();
    QJsonArray json_array;

    static json_return readJson()
    {
        json_return res;
        QFile file(QCoreApplication::applicationDirPath() + "/data.json");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning("Couldn't open file.");
        }
        QByteArray raw_data = file.readAll();
        QJsonDocument json_doc = QJsonDocument::fromJson(raw_data);
        QJsonObject json_object = json_doc.object();

        res.total = json_object["total"].toInt();
        res.parents = json_object["parents"].toInt();
        res.moneybox = json_object["moneybox"].toInt();
        res.total_last_month = json_object["total_last_month"].toInt();

        QJsonArray json_array = json_object["incomes"].toArray();
        QMap<QString, int> cur_map;
        for(const QJsonValue &val : json_array)
        {
            QJsonObject jsonobject = val.toObject();
            cur_map["month"] = jsonobject["month"].toInt();
            cur_map["month_total"] = jsonobject["month_total"].toInt();
            cur_map["r"] = jsonobject["r"].toInt();
            cur_map["g"] = jsonobject["g"].toInt();
            cur_map["b"] = jsonobject["b"].toInt();
            res.map.append(cur_map);
        }
        return res;
    }

    static void write_json(json_return json_data)
    {
        QJsonObject main_obj;
        QJsonArray array_obj;
        for(int i = 0; i < json_data.map.length(); i++)
        {
            QJsonObject* obj = new QJsonObject();
            obj->insert("month", json_data.map[i]["month"]);
            obj->insert("month_total", json_data.map[i]["month_total"]);
            obj->insert("r", json_data.map[i]["r"]);
            obj->insert("g", json_data.map[i]["g"]);
            obj->insert("b", json_data.map[i]["b"]);
            array_obj.append(*obj);
        }
        main_obj.insert("incomes", array_obj);
        main_obj.insert("parents", json_data.parents);
        main_obj.insert("moneybox", json_data.moneybox);
        main_obj.insert("total", json_data.total);
        main_obj.insert("total_last_month", json_data.total_last_month);
        QJsonDocument json_out(main_obj);
        QFile file_write(QCoreApplication::applicationDirPath() + "/data.json");
        if (!file_write.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning("Couldn't open file.");
        }
        file_write.write(json_out.toJson());
        file_write.close();
    }

};

#endif // JSONHANDLER_H

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
    QVector<QMap<QString, int>> map;
}json_return;

class JsonHandler
{
public:
    void writeJson();

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

        QJsonArray json_array = json_object["incomes"].toArray();
        QMap<QString, int> cur_map;
        for(const QJsonValue &val : json_array)
        {
            QJsonObject jsonobject = val.toObject();
            cur_map["month"] = jsonobject["month"].toInt();
            cur_map["month_total"] = jsonobject["month_total"].toInt();
            cur_map["r"] = jsonobject["red"].toInt();
            cur_map["g"] = jsonobject["green"].toInt();
            cur_map["b"] = jsonobject["blue"].toInt();
            res.map.append(cur_map);
        }
        return res;
    }

};

#endif // JSONHANDLER_H

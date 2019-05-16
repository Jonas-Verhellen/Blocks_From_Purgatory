#include "savegame.h"
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <iostream>
using namespace std;


Savegame::Savegame() :
    my_score(0),
    my_name("Oh Good. For a moment there, I thought we were in trouble."),
    my_lines(0),
    my_spm(0)
{}

Savegame::Savegame(const QString &name, int score):
    my_name(name),
    my_score(score)
{}

QString Savegame::name() const
{
    return my_name;
}

void Savegame::setName(const QString &name)
{
    my_name = name;
}

int Savegame::score() const
{
    return my_score;
}

void Savegame::setScore(int score)
{
    my_score = score;
}

void Savegame::setLines(int lines)
{
    my_lines = lines;
}

int Savegame::lines() const
{
    return my_lines;
}

void Savegame::setSpm(int spm)
{
    my_spm = spm;
}

float Savegame::spm() const
{
    return my_spm;
}

void Savegame::read(const QJsonObject &json)
{
    my_name = json["name"].toString();
    my_score = json["score"].toDouble();
    my_lines = json["lines"].toDouble();
    my_spm = json["lines"].toDouble();
}

void Savegame::write(QJsonObject &json) const
{
    time_t result = time(nullptr);
    json["name"] = my_name;
    json["score"] = my_score;
    json["lines"] = my_lines;
    json["spm"] = my_spm;
    json["timestamp"] =  asctime(std::localtime(&result));
}


bool Savegame::storeGame(QJsonObject &json) const
{
    QFile saveFile(QStringLiteral("save_tetris.json"));
    bool a = saveFile.open(QIODevice::WriteOnly);
    if (!a)
    {
        return false;
    }

    QJsonDocument saveDoc(json);
    saveFile.write(saveDoc.toJson());
    return true;
}

QByteArray Savegame::datastream(QJsonObject &json) const
{
    QJsonDocument Game_Data_doc (json);
    return Game_Data_doc.toJson();
}

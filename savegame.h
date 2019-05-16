#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <QJsonObject>
#include <QString>
#include <ctime>

class Savegame
{
public:
    Savegame();
    Savegame(const QString &name, int score);

    QString name() const;
    void setName(const QString &name);

    int score() const;
    int lines() const;
    float spm() const;

    void setScore(int score);
    void setLines(int lines);
    void setSpm(int spm);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;

    bool storeGame(QJsonObject &json) const;
    QByteArray datastream(QJsonObject &json) const;

private:
    QString my_name;
    int my_score;
    int my_lines;
    float my_spm;
};

#endif // SAVEGAME_H

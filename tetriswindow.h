#ifndef TETRISWINDOW_H
#define TETRISWINDOW_H

#include <QFrame>
#include <QWidget>
#include <QMediaPlayer>
#include "tetrisboard.h"
#include "savegame.h"

class QLCDNumber;
class QLabel;
class QPushButton;
class QTcpServer;
class QNetworkSession;

class TetrisWindow : public QWidget
{
    Q_OBJECT

public:
    TetrisWindow();

private slots:
    void session_opened();
    void send_data();
    void save_data();
private:
    QLabel *createLabel(const QString &text);

    TetrisBoard *board;

    QMediaPlayer *player;
    QMovie *movie;

    QLCDNumber *scoreLcd;
    QLCDNumber *linesLcd;

    QPushButton *startButton;
    QPushButton *quitButton;

    QLabel *statusLabel;
    QTcpServer *tcpServer;
    QNetworkSession *networkSession;

    Savegame *Saver;
    QJsonObject *Game_data;
};

#endif

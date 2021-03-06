#include "tetriswindow.h"
#include "ui_tetriswindow.h"
#include <QtWidgets>
#include <QtNetwork>
#include <iostream>
#include <exception>
#include "savegame.h"
using namespace std;
TetrisWindow::TetrisWindow()
{

    // basics

    setWindowTitle("BLOCKS FROM PURGATORY");
    this->setFixedSize(550,445);
    board = new TetrisBoard;
    //board->setFixedSize(220+2,420+2);
    board->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


    // muziek

    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("/Users/Jonas/Music/Blokken.mp3"));
    player->setVolume(5); //volume nog aanpassen

    // Gif

    movie = new QMovie("/Users/Jonas/Music/giphy.gif");
    QLabel *processLabel = new QLabel(this);
    QSize const *size = new QSize(150,75);
    processLabel->setMovie(movie);
    movie->setScaledSize(*size);

    // Buttons

    startButton = new QPushButton("Start");
    startButton->setFocusPolicy(Qt::NoFocus);
    quitButton = new QPushButton("Quit");
    quitButton->setFocusPolicy(Qt::NoFocus);

    connect(startButton, SIGNAL(clicked()), board, SLOT(start()));
    connect(startButton, SIGNAL(clicked()), player, SLOT(play()));
    connect(startButton, SIGNAL(clicked()), movie, SLOT(start()));
    connect(quitButton , SIGNAL(clicked()), qApp, SLOT(quit()));

    // LCDS

    scoreLcd = new QLCDNumber(2);
    linesLcd = new QLCDNumber(2);

    connect(board, SIGNAL(score_changed(int)), scoreLcd, SLOT(display(int)));
    connect(board, SIGNAL(lines_changed(int)), linesLcd, SLOT(display(int)));

    // Networking

    statusLabel = new QLabel;
    tcpServer= NULL;
    networkSession = NULL;
    session_opened();
    connect(tcpServer, &QTcpServer::newConnection, this, &TetrisWindow::send_data);

    //Saving the game

    Saver = new Savegame;
    Game_data = new QJsonObject;
    QString Name = QHostInfo::localHostName(); //DNS
    Saver->setName(Name);
    Saver->write(*Game_data);
    connect(board, &TetrisBoard::lines_changed, this, &TetrisWindow::save_data);

    //Layout

    QGridLayout *layout= new QGridLayout;
    layout->addWidget(startButton, 4, 0);
    layout->addWidget(quitButton, 5, 0);
    layout->addWidget(statusLabel, 2, 0);
    layout->addWidget(createLabel("Score"), 0, 2);
    layout->addWidget(createLabel("# Lines Removed"), 2, 2);

    layout->addWidget(board, 0, 1, 6, 1);

    layout->addWidget(scoreLcd, 1, 2);
    layout->addWidget(linesLcd, 3, 2);
    layout->addWidget(processLabel,3,0);

    setLayout(layout);
}

void  TetrisWindow::save_data()
{
    Saver->setScore(scoreLcd->intValue());
    Saver->setLines(linesLcd->intValue());
    Saver->setSpm(board->Get_Spm());
    Saver->write(*Game_data);
    bool check =  Saver->storeGame(*Game_data);
    if (check == FALSE)
    {
        QMessageBox::warning(this,"Tetris Server","save_tetris.json could not be written on.");
    }
}

QLabel *TetrisWindow::createLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    return lbl;
}

void TetrisWindow::session_opened()
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen()) {
        QMessageBox::critical(this, "Tetris Server",tr("Unable to start the Tetris server: %1.").arg(tcpServer->errorString()));
        close();
        return;
    }

    // on an arbitrary port
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address())
        {
            //use the first non-localhost (IPv4) that is not an 169.254 ("link local") adresses
            cout << ipAddressesList.at(i).toString().toUtf8().constData() << endl;
            ipAddress = ipAddressesList.at(i).toString();
            QString str = ipAddress.left(7);
            if(str != "169.254") break;
        }
    }

    if (ipAddress.isEmpty())
    {
        QMessageBox::information(this, "Tetris Server", "No non-local IP Address found, used localhost.");
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }

    statusLabel->setText(tr("Purgatory is located on\n\nIP: %1\nport: %2\n\n").arg(ipAddress).arg(tcpServer->serverPort()));
}

void TetrisWindow::send_data()
{
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected, clientConnection, &QObject::deleteLater);

    QByteArray jSon_To_Send =  Saver->datastream(*Game_data);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << quint16( 0 ) //space for a 16 bit int = the size of the data block
        << jSon_To_Send;
    out.device()->seek( 0 );
    out << quint16( block.size() - sizeof(quint16) );

    try
      {
        clientConnection->write(block);
      }
    catch(exception& e)
        {
            QMessageBox::critical(this,"Tetris Server",tr("Writing failed: standard exception: %1.").arg(e.what()));
        }
    clientConnection->disconnectFromHost();
}

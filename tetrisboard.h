#ifndef TETRISBOARD_H
#define TETRISBOARD_H

#include <QBasicTimer>
#include <QFrame>
#include <QPointer>
#include <QGesture>
#include "tetrispiece.h"

class QLabel;

class TetrisBoard: public QFrame
{
    Q_OBJECT

public:
    TetrisBoard(QWidget *parent = 0);
    float Get_Spm() const;

public slots:
    void start();

signals:
    void score_changed(int Score);
    void lines_changed(int Lines);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE Q_DECL_FINAL;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE Q_DECL_FINAL;
    void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE Q_DECL_FINAL;

     bool event(QEvent *event);
     bool gestureEvent(QGestureEvent *event);
     void swipeTriggered(QSwipeGesture *gesture);

    QSize minimumSizeHint() const Q_DECL_OVERRIDE Q_DECL_FINAL;
    QSize frameSize() const ;
    QSize maximumSize() const;

private:
    QBasicTimer timer;
    TetrisPiece current_piece;
    vector< vector<int> > board; //begint links boven te tellen

    bool started;
    bool after_block;

    int lines;
    int score;

    int keystrokes;
    float spm;

    int move_piece(int offset_x, int offset_y);
    int gravity();
    int rotate();
    void new_piece();

    void store_piece_in_play();
    void erase_piece_in_play();

    void delete_line(int line);
    int delete_lines();

    bool upper_line_check();
    int legal_place();
};

#endif // TETRISBOARD_H

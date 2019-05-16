#include <QtWidgets>
#include <iostream>
#include "tetrisboard.h"
#include "tetrispiece.h"
using namespace std;
TetrisBoard::TetrisBoard(QWidget *parent)
    : QFrame(parent)
{
    setFrameStyle(QFrame::StyledPanel);
    setFocusPolicy(Qt::StrongFocus);
    started = false;
    after_block = false;
    for (int i = 0; i < 26; i++) //26 op 16 + speeldomein : 23 op 11 maar de rest zijn borders
    {
        vector<int> temporary;
        for (int j = 0; j <16; j++)
        {
            temporary.push_back(0);
        }
        board.push_back(temporary);
    }
}

float TetrisBoard::Get_Spm() const
{
    return spm;
}

void TetrisBoard::start()
{
    started = true;
    lines = 0;
    score = 0;
    keystrokes = 0;
    spm = 0;
    setFocusPolicy(Qt::StrongFocus);

    emit lines_changed(lines);
    emit score_changed(score);
    for (int i = 0; i < 26; i++)
    {
        for (int j = 0; j <16; j++)
        {
            board[i][j] = 0;
        }
    }
    new_piece();
    update();
    timer.start(125, this); //timeout is in milliseconds
}

QSize TetrisBoard:: frameSize() const //moet met getallen zijn.
{
      return QSize(220 + 2 , 420 + 2);
}

QSize TetrisBoard::minimumSizeHint() const
{
    return QSize(220 + 2 , 420 + 2);
}

QSize TetrisBoard::maximumSize() const
{
    return QSize(220 + 2 , 420 + 2);
}

void TetrisBoard::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    vector<QColor> color_vector ;
    color_vector.push_back(Qt::blue);
    color_vector.push_back(Qt::red);
    color_vector.push_back(Qt::yellow);
    color_vector.push_back(Qt::green);
    color_vector.push_back(Qt::magenta);
    color_vector.push_back(Qt::cyan);
    color_vector.push_back(Qt::gray);

    QRect rect = contentsRect();

    for (int i = 0; i < 11; ++i)
    {
        for (int j = 0; j < 23; ++j)
        {
            if(board[j+3][i+3] != 0)
            {
                painter.fillRect( (rect.left() + i * 20) + 1, (j * 20) + 1, (20 - 2) , (20 - 2),color_vector[board[j+3][i+3] -1]);
            }
        }
    }
}


void TetrisBoard::keyPressEvent(QKeyEvent *event)
{
    if (started == false || event->isAutoRepeat() )
    {
        QFrame::keyPressEvent(event); // pass to standard keypressevent handler
        return;
    }
    keystrokes ++;
    switch (event->key())
    {
        case Qt::Key_Left:
            move_piece(-1,0);
            repaint();
            break;
        case Qt::Key_Right:
            move_piece(1,0);
            repaint();
            break;
        case Qt::Key_Up:
            rotate();
            repaint();
            break;
        default:
            QFrame::keyPressEvent(event);
    }
}


void TetrisBoard::timerEvent(QTimerEvent *event)
{
    if (Q_LIKELY(event->timerId() == timer.timerId())) //om de juiste timer te hebben, een beetje overkill.
    {
        if (after_block)
        {
            after_block = false;
            if(upper_line_check())
            {
                started = false;
            }
            else
            {
                delete_lines();
                repaint();
                new_piece();
                update();
            }
        }
        else
        {
            gravity();
            repaint();
        }
        static int counter = 0;
        static float counter2 = 0;
        counter++;
        if(counter == 80) //every 10 seconds
        {
            counter = 0;
            counter2++;
            spm = keystrokes*(6/counter2);
        }
    }
    else
    {
        QFrame::timerEvent(event);
    }
}

bool TetrisBoard::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
    {
        cout << "Gesture Detected" <<endl;
        return gestureEvent(static_cast<QGestureEvent*>(event));
    }
    return QWidget::event(event);
}

bool TetrisBoard::gestureEvent(QGestureEvent *event)
{
    if (QGesture *swipe = event->gesture(Qt::SwipeGesture))
        swipeTriggered(static_cast<QSwipeGesture *>(swipe));
    return true;
}


void TetrisBoard::swipeTriggered(QSwipeGesture *gesture)
{
    if (gesture->state() == Qt::GestureFinished) {
        if (gesture->horizontalDirection() == QSwipeGesture::Left)

        switch (gesture->horizontalDirection())
        {
            case QSwipeGesture::Left:
                move_piece(-1,0);
                repaint();
                break;
            case QSwipeGesture::Right:
                move_piece(1,0);
                repaint();
                break;
            case QSwipeGesture::Up:
                rotate();
                repaint();
                break;
            default:
                break;
        }
    }
}

void TetrisBoard::new_piece()
{
    current_piece.Set_Type(qrand() % 7);
    current_piece.Set_x(5);
    current_piece.Set_y(2);
    store_piece_in_play();
}

int TetrisBoard::move_piece(int offset_x, int offset_y)
{
    int Px = current_piece.Get_x();
    int Py = current_piece.Get_y();
    erase_piece_in_play();
    current_piece.Set_x(Px + offset_x);
    current_piece.Set_y(Py + offset_y);
    int state = legal_place();
    if (state== 0 || state== 1 || state== 2)
    {
        current_piece.Set_x(Px);
        current_piece.Set_y(Py);
    }
    store_piece_in_play();
    return state;
}

int TetrisBoard::rotate()
{
    erase_piece_in_play();
    current_piece.Rotate();
    int state = legal_place();
    if (state== 0 || state== 1 || state== 2)
    {
        current_piece.Rotate();
        current_piece.Rotate();
        current_piece.Rotate();
    }
    store_piece_in_play();
    return state;
}

int TetrisBoard::gravity()
{
    int Py = current_piece.Get_y();
    erase_piece_in_play();
    current_piece.Set_y(Py + 1);
    int state = legal_place();
    if(state == 1  || state== 2)
    {
        current_piece.Set_y(Py);
        store_piece_in_play();
        after_block = true;
    }
    else
    {
        store_piece_in_play();
    }
    return state;
}

void TetrisBoard::store_piece_in_play ()
{
    int Px = current_piece.Get_x();
    int Py = current_piece.Get_y();
    for(int i = 0; i<5; i++)
    {
        for(int j = 0; j<5; j++)
        {
            if(current_piece.Get_Form()[i][j] != 0)
            {
                board[Py+i][Px+j]  = current_piece.Get_Form()[i][j];
            }
        }
    }
}


void TetrisBoard::erase_piece_in_play()
{
    int Px = current_piece.Get_x();
    int Py = current_piece.Get_y();
    for (int i = Px, k = 0; i < Px + 5; i++, k++)
    {
        for (int j = Py, l = 0; j < Py + 5; j++, l++)
        {
                if(current_piece.Get_Form()[l][k] != 0)
                {
                    board[j][i] = 0;
                }
         }
    }
}

int TetrisBoard::legal_place()
{
        int Px = current_piece.Get_x();
        int Py = current_piece.Get_y();
        for (int i = Px, k = 0; i < Px + 5; i++, k++) // k en l houden bij waar in de block dat het probleem zich stelt
        {
            for (int j = Py, l = 0; j < Py + 5; j++, l++)
            {
                //controleren op de randen
                if ( i < 3 || i > 13)
                {//controleren op de juiste spot in de block structuur
                    if(current_piece.Get_Form()[l][k] != 0){return 0;}
                }
                if ( j > 23)
                {//controleren op de juiste spot in de block structuur
                    if(current_piece.Get_Form()[l][k] != 0){return 1;}
                }

                //controleren op botsing met andere blokjes
                if (Q_LIKELY(j > 0))
                {//controleren op de juiste spot in de block structuur
                    if ((current_piece.Get_Form()[l][k] != 0)&&(board[j][i]!=0)){return 2;}
                }
            }
        }
        return 3; // alles ok
}



bool TetrisBoard::upper_line_check()
{
    for (int i = 3; i < 14; i++)
    {
        if (board[5][i] != 0)
        {
            return true;
        }
    }
    return false; //true: is game over
}

void TetrisBoard::delete_line(int line)
{
    for (int i = line; i > 0; i--)
    {
        for (int j = 3; j < 14; j++)
        {
            board[i][j] = board[i-1][j];
        }
    }
}

int TetrisBoard::delete_lines()
{
    int lines_local = 0;
    for (int i = 4; i < 24; i++)
    {
        int z = 0;
        for (int j = 3; j < 14; j++)
        {
            if (board[i][j] == 0) {break;}
            z++;
        }
        if (z == 11)
        {
            delete_line(i);
            lines_local++;
        }
    }
    lines = lines + lines_local;
    if (lines_local % 4 == 0 && Q_LIKELY(lines_local != 0)){score = score +  4*lines_local;}
    else {score = score +  lines_local;}
    emit lines_changed(lines);
    emit score_changed(score);
    if (lines > 19)
    {
        started = false;
    }
    return lines;
}

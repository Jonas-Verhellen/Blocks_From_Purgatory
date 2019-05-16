#include "tetrispiece.h"
#include <vector>
#include <iostream>
using namespace std;

TetrisPiece::TetrisPiece()
{
    Piece_x = 0;
    Piece_y = 0;
    for (int i = 0; i < 5; i++)
    {
        vector<int> temp;
        for (int j = 0; j < 5; j++)
        {
            temp.push_back(0);
        }
        Piece_Form.push_back(temp);
    }
}

void TetrisPiece::Set_Type(int Tetrominos)
{
    Piece_Type = Tetrominos;
    //5*5 matrices for each trinomino.
    const int BLOCKLIST[7][5][5] = {
        {
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0},
         {0, 1, 1, 1, 1},
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0}
         },
        {
         {0, 0, 0, 0, 0},
         {0, 0, 2, 0, 0},
         {0, 0, 2, 0, 0},
         {0, 2, 2, 0, 0},
         {0, 0, 0, 0, 0}
         },
        {
         {0, 0, 0, 0, 0},
         {0, 0, 3, 0, 0},
         {0, 0, 3, 0, 0},
         {0, 0, 3, 3, 0},
         {0, 0, 0, 0, 0}
         },
        {
         {0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0},
         {0, 0, 4, 4, 0},
         {0, 0, 4, 4, 0},
         {0, 0, 0, 0, 0}
         },
        {
         {0, 0, 0, 0, 0},
         {0, 0, 5, 0, 0},
         {0, 0, 5, 5, 0},
         {0, 0, 0, 5, 0},
         {0, 0, 0, 0, 0}
         },
        {
         {0, 0, 0, 0, 0},
         {0, 0, 6, 0, 0},
         {0, 0, 6, 6, 0},
         {0, 0, 6, 0, 0},
         {0, 0, 0, 0, 0}
         },
        {
         {0, 0, 0, 0, 0},
         {0, 0, 0, 7, 0},
         {0, 0, 7, 7, 0},
         {0, 0, 7, 0, 0},
         {0, 0, 0, 0, 0}
         }
    };

if(Tetrominos < 7)
{
         for (int i = 0; i < 5; i++)
         {
             for (int j = 0; j < 5; j++)
             {
                 Piece_Form[i][j] = BLOCKLIST[Tetrominos][i][j];
             }
         }
}
}

void TetrisPiece::Rotate()
{
    vector< vector<int> > old_vector;
    old_vector = Piece_Form;
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<5;j++){Piece_Form[4-i][j]= old_vector[j][i];};
    };
}

//Get
int TetrisPiece::Get_x()
{
    return Piece_x;
}
int TetrisPiece::Get_y()
{
    return Piece_y;
}
vector< vector<int> > TetrisPiece::Get_Form()
{
    return Piece_Form;
}
int TetrisPiece::Get_Type()
{
    return Piece_Type;
}

//Set
void TetrisPiece::Set_x(int X)
{
    Piece_x = X;
}
void TetrisPiece::Set_y(int Y)
{
    Piece_y = Y;

}

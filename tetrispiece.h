#ifndef TETRISPIECE_H
#define TETRISPIECE_H
using namespace std;
#include <vector>

class TetrisPiece
{
private:
    int Piece_x;
    int Piece_y;
    char Piece_Type;
    vector< vector<int> > Piece_Form;

public:
    TetrisPiece();

    int Get_x(); // links boven
    int Get_y();
    int Get_Type();
    vector< vector<int> > Get_Form();

    void Set_x(int X);
    void Set_y(int Y);
    void Set_Type(int Tetrominos);

    void Rotate();
};
#endif // TETRISPIECE_H

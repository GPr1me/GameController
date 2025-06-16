#ifndef TETRIS_H
#define TETRIS_H

#include "../Program/Program.h"
#include "../WS2812B_matrix/WS2812BMatrix.h"
#include "Models/AnimationLayer/AnimationLayer.h"
#include "../Button/Button.h"

class Tetris : public Program {
    // Tetris game class
    // This class handles the Tetris game logic and rendering on the WS2812B matrix
    // It uses a 2D array to represent the game board and a struct to represent the current piece
    // The game supports basic controls for moving and rotating pieces
    // The game can be started and run using the begin() and run() methods
public:
    Tetris(WS2812BMatrix* matrix, AnimationLayer* animationLayer,  Button* left, Button* right, Button* rotate, Button* drop);

    void setup() override;
    void run() override;
    void drawTitleScreen(bool isSelected) override;

    void onLeft();
    void onRight();
    void onRotate();
    void onDrop();

private:
    WS2812BMatrix* _matrix;
    AnimationLayer* _animationLayer;
    Button* _btnLeft;
    Button* _btnRight;
    Button* _btnRotate;
    Button* _btnDrop;

    static const uint8_t WIDTH = 8;
    static const uint8_t HEIGHT = 16;
    uint32_t _board[WIDTH][HEIGHT];

    enum GameState { NORMAL, LOCKED, CLEARED, SPAWNED, OVER };
    GameState _state;
    

    struct Piece {
        int type;
        int rotation;
        int x, y;
    } _currentPiece;

    void _spawnPiece();
    bool _movePiece(int dx, int dy);
    bool _rotatePiece();
    void _lockPiece();
    bool _clearLines();
    bool _checkCollision(int nx, int ny, int nrot);
    void _drawBoard();
    void _drawPiece();
    void _clearBoard();
    void _resetGame();
    void _setState(GameState state);
    void _displayScore();

    unsigned long _lastDropTime;
    unsigned long _dropInterval;

    int _score;
    unsigned long _stateTime;
    bool _firstPassSetup;
};

#endif // TETRIS_H
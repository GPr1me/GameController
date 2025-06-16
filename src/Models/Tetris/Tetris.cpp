#include "Tetris.h"
#include "TetrominoData.h"

static const uint32_t EMPTY = 0x000000; // Color for empty cells

const uint16_t logo5x8[5 * 8] = {
    CYAN,   FLUSH_ORANGE,   FLUSH_ORANGE,   FLUSH_ORANGE,   FLUSH_ORANGE,   RED,              YELLOW, YELLOW,
    CYAN,   GREEN,          GREEN,          GREEN,          RED,            RED,              YELLOW, YELLOW,
    CYAN,   CYAN,           GREEN,          BLACK,          RED,            ROSE,               ROSE,   ROSE,
    YELLOW, YELLOW,         BLACK,          BLACK,          BLACK,          ELECTRIC_INDIGO,    ELECTRIC_INDIGO, ROSE,
    YELLOW, YELLOW,         FLUSH_ORANGE,   FLUSH_ORANGE,   FLUSH_ORANGE,   FLUSH_ORANGE,       ELECTRIC_INDIGO, ELECTRIC_INDIGO
};
const uint8_t digitFont[10][5] = {
    {0b111, 0b101, 0b101, 0b101, 0b111}, // 0
    {0b010, 0b110, 0b010, 0b010, 0b111}, // 1
    {0b111, 0b001, 0b111, 0b100, 0b111}, // 2
    {0b111, 0b001, 0b111, 0b001, 0b111}, // 3
    {0b101, 0b101, 0b111, 0b001, 0b001}, // 4
    {0b111, 0b100, 0b111, 0b001, 0b111}, // 5
    {0b111, 0b100, 0b111, 0b101, 0b111}, // 6
    {0b111, 0b001, 0b010, 0b100, 0b100}, // 7
    {0b111, 0b101, 0b111, 0b101, 0b111}, // 8
    {0b111, 0b101, 0b111, 0b001, 0b111}  // 9
};


Tetris::Tetris(WS2812BMatrix* _matrix, AnimationLayer* animationLayer, Button* left, Button* right, Button* rotate, Button* drop)
    : _matrix(_matrix), _animationLayer(animationLayer), _btnLeft(left), _btnRight(right), _btnRotate(rotate), _btnDrop(drop), _lastDropTime(0), _dropInterval(1000)
{
    _isSetup = false;
    _clearBoard();
    _lastDropTime = 0;

    _state = NORMAL;
    _score = 0;
    _stateTime = 0;
    _firstPassSetup = true;
}

void Tetris::_resetGame() {
    _clearBoard();
    _lastDropTime = millis();

    _state = NORMAL;
    _score = 0;
    _stateTime = _lastDropTime;
    _firstPassSetup = true;

    _spawnPiece();
}

void Tetris::setup() {
    _animationLayer->animateMenuTitleBlinkAndGrow(logo5x8, 8, 5, CYAN, FLUSH_ORANGE, 6, 12, 100, 4);
    _animationLayer->clear();

    _btnLeft->setCallback([this](Button&) { onLeft(); });
    _btnLeft->setDebounceDelay(250);
    _btnRight->setCallback([this](Button&) { onRight(); });
    _btnRight->setDebounceDelay(250);
    _btnRotate->setCallback([this](Button&) { onRotate(); });
    _btnRotate->setDebounceDelay(250);
    _btnDrop->setCallback([this](Button&) { onDrop(); });
    _btnDrop->disableInterrupts();
    
    _resetGame();

    _isSetup = true;
    Serial.println("Tetris setup complete.");
}

void Tetris::run() {

    unsigned long now = millis();

    switch (_state) {
        case NORMAL:
            if (_firstPassSetup) {
                _lastDropTime = millis();
                _firstPassSetup = false;
            }
            if (now - _lastDropTime > _dropInterval) {
                if (!_movePiece(0, 1)) {
                    _lockPiece();
                    _setState(LOCKED);
                }
                _lastDropTime = now;
            }
            _drawBoard();
            _drawPiece();
            break;

        case LOCKED:
            if (_firstPassSetup) {
                _drawBoard();
                _drawPiece();
                _firstPassSetup = false;
            }
            if (now - _stateTime > 100) {
                if(_clearLines()) {
                    _setState(CLEARED);
                } else {
                    _setState(NORMAL);
                    _spawnPiece();
                }
            }
            break;

        case CLEARED:
            if (_firstPassSetup) {
                _drawBoard();
                _firstPassSetup = false;
            }
            if (now - _stateTime > 100) { // Show cleared lines for 100ms
                _setState(NORMAL);
                _spawnPiece();
            }
            break;
        case OVER:
            if (_firstPassSetup) {
                _drawBoard();
                _firstPassSetup = false;
            }
            if (now - _stateTime < 3000) {
                _displayScore();
                break;
            }
            _resetGame();
            break;
    }

    _matrix->show();

    _dropInterval = 1000;

    _btnLeft->run();
    _btnRight->run();
    _btnRotate->run();
    _btnDrop->run();
}

void Tetris::drawTitleScreen(bool isSelected) {
    uint16_t bandColor = isSelected ? CYAN : BLACK;
    _animationLayer->drawMenuTitle(logo5x8, 8, 5, bandColor, FLUSH_ORANGE, 2);
}

void Tetris::onLeft()   { _movePiece(-1, 0); }
void Tetris::onRight()  { _movePiece(1, 0); }
void Tetris::onRotate() { _rotatePiece(); }
void Tetris::onDrop()   { _dropInterval = 100; } // Speed up drop for quick drop

void Tetris::_spawnPiece() {
    _currentPiece.type = random(0, 7);
    _currentPiece.rotation = 0;
    _currentPiece.x = WIDTH / 2 - 2;
    _currentPiece.y = 0;
    
    // Game over if the new piece collides immediately
    if (_checkCollision(_currentPiece.x, _currentPiece.y, _currentPiece.rotation)) {
        _setState(OVER);
    }
}

bool Tetris::_movePiece(int dx, int dy) {
    if (!_checkCollision(_currentPiece.x + dx, _currentPiece.y + dy, _currentPiece.rotation)) {
        _currentPiece.x += dx;
        _currentPiece.y += dy;
        return true;
    }
    return false;
}

bool Tetris::_rotatePiece() {
    int newRot = (_currentPiece.rotation + 1) % 4;
    if (!_checkCollision(_currentPiece.x, _currentPiece.y, newRot)) {
        _currentPiece.rotation = newRot;
        return true;
    }
    return false;
}

void Tetris::_lockPiece() {
    for (int dy = 0; dy < 4; ++dy) {
        for (int dx = 0; dx < 4; ++dx) {

            if (tetromino[_currentPiece.type][ _currentPiece.rotation][dy][dx]) {

                int boardX = _currentPiece.x + dx;
                int boardY = _currentPiece.y + dy;

                // Only lock if inside the _board
                if (boardX >= 0 && boardX < WIDTH && boardY >= 0 && boardY < HEIGHT) {
                    _board[boardX][boardY] = pieceColors[_currentPiece.type];
                }
            }
        }
    }
}

bool Tetris::_clearLines() {
    int linesCleared = 0;

    for (int y = HEIGHT - 1; y >= 0; --y) {
        bool full = true;
        for (int x = 0; x < WIDTH; ++x) {
            if (_board[x][y] == EMPTY) {
                full = false;
                break;
            }
        }
        if (full) {
            // Move all rows above down by one
            for (int yy = y; yy > 0; --yy) {
                for (int x = 0; x < WIDTH; ++x) {
                    _board[x][yy] = _board[x][yy - 1];
                }
            }
            // Clear the top row
            for (int x = 0; x < WIDTH; ++x) {
                _board[x][0] = EMPTY;
            }
            // Check the same row again (since it now contains the row above)
            ++y;
            ++linesCleared;
        }
    }
    
    
    // Custom scoring: 1, 3, 5, 8
    switch (linesCleared) {
        case 1: _score += 1; break;
        case 2: _score += 3; break;
        case 3: _score += 5; break;
        case 4: _score += 8; break;
        default: break;
    }

    return linesCleared > 0;
}

bool Tetris::_checkCollision(int nx, int ny, int nrot) {
    int type = _currentPiece.type;
    for (int dy = 0; dy < 4; ++dy) {
        for (int dx = 0; dx < 4; ++dx) {
            if (tetromino[type][nrot][dy][dx]) {
                int boardX = nx + dx;
                int boardY = ny + dy;
                // Check out of bounds
                if (boardX < 0 || boardX >= WIDTH || boardY < 0 || boardY >= HEIGHT)
                    return true;
                // Check collision with filled cells
                if (_board[boardX][boardY] != EMPTY)
                    return true;
            }
        }
    }
    return false;
}

void Tetris::_drawBoard() {
    _matrix->clear();
    for (int x = 0; x < WIDTH; ++x)
        for (int y = 0; y < HEIGHT; ++y)
            if (_board[x][y])
                _matrix->drawPixel(x, y, _board[x][y]);
}

void Tetris::_drawPiece() {
    int type = _currentPiece.type;
    int rot = _currentPiece.rotation;
    int px = _currentPiece.x;
    int py = _currentPiece.y;

    uint32_t color = pieceColors[type];

    for (int dy = 0; dy < 4; ++dy) {
        for (int dx = 0; dx < 4; ++dx) {
            if (tetromino[type][rot][dy][dx]) {
                int boardX = px + dx;
                int boardY = py + dy;
                if (boardX >= 0 && boardX < WIDTH && boardY >= 0 && boardY < HEIGHT) {
                    _matrix->drawPixel(boardX, boardY, color);
                }
            }
        }
    }
}

void Tetris::_clearBoard() {
    for (int x = 0; x < WIDTH; ++x)
        for (int y = 0; y < HEIGHT; ++y)
            _board[x][y] = EMPTY;
}

void Tetris::_displayScore() {
    char buf[6];
    snprintf(buf, sizeof(buf), "%d", _score);
    int numDigits = strlen(buf);

    int digitWidth = 3, digitHeight = 5;
    int x = (WIDTH - digitWidth) / 2; // center horizontally

    // Center vertically
    int totalHeight = numDigits * digitHeight;
    int y0 = (HEIGHT - totalHeight) / 2;
    
    // Erase area around the score (draw a black rectangle)
    for (int col = 0; col < digitWidth; ++col) {
        for (int row = 0; row < totalHeight; ++row) {
            int px = x + col;
            int py = y0 + row;
            if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
                _matrix->drawPixel(px, py, BLACK);
            }
        }
    }

    for (int d = 0; d < numDigits; ++d) {
        int digit = buf[d] - '0';
        for (int row = 0; row < digitHeight; ++row) {
            for (int col = 0; col < digitWidth; ++col) {
                if (digitFont[digit][row] & (1 << (2 - col))) {
                    _matrix->drawPixel(x + col, y0 + d * digitHeight + row, FLUSH_ORANGE);
                }
            }
        }
    }
}

void Tetris::_setState(GameState state) {
    _state = state;
    _stateTime = millis();
    _firstPassSetup = true;
}
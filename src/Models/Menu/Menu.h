#ifndef MENU_H
#define MENU_H

#include "../Program/Program.h"
#include "../Button/Button.h"
#include "../AnimationLayer/AnimationLayer.h"
#include "../QuizGame/QuizGame.h"
#include "../Tetris/Tetris.h"

class Menu : public Program {
public:
    Menu(Button* btnNavigateDecrement, 
         Button* btnNavigateIncrement, 
         Button* btnSelect, 
         Button* btnUnselect,
         AnimationLayer* animationLayer,
         WS2812BMatrix* matrix,
         QuizGame* quizGame,
         Tetris* tetris);

    void setup() override;
    void run() override;
    void drawTitleScreen(bool isSelected) override;

    Program* getSelectedProgram() const;

    void setQuizGame(QuizGame* quizGame);
    void setTetris(Tetris* tetris);

private:
    Button* _btnNavigateDecrement;
    Button* _btnNavigateIncrement;
    Button* _btnSelect;
    Button* _btnUnselect;
    AnimationLayer* _animationLayer;
    WS2812BMatrix* _matrix;

    int _selectedIndex;
    Program* _selected;
    QuizGame* _quizGame;
    Tetris* _tetris;

    void _navigate(int direction);
    void _select();
    void _unselect();
    void _displayMenu();
};

#endif // MENU_H

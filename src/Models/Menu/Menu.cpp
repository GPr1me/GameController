#include "Menu.h"

#define PROGRAM_NB 2

Menu::Menu(Button* button1, Button* button2, Button* button3, Button* button4, 
		   AnimationLayer* animationLayer, WS2812BMatrix* matrix, 
		   QuizGame* quizGame, Tetris* tetris)
	: _btnNavigateDecrement(button1), _btnNavigateIncrement(button2), _btnSelect(button3), _btnUnselect(button4),
	  _animationLayer(animationLayer), _matrix(matrix), _selectedIndex(0), 
	  _quizGame(quizGame), _tetris(tetris)
{
}

void Menu::setup() {
	_btnNavigateDecrement->setCallback([this](Button&) { _navigate(-1); });
	_btnNavigateIncrement->setCallback([this](Button&) { _navigate(1); });
	_btnSelect->setCallback([this](Button&) { _select(); });
	_btnSelect->setDebounceDelay(500);
	_btnUnselect->setCallback([this](Button&) { _unselect(); });
	
	_displayMenu();
}

void Menu::run() {
	if (_selected && _selected->isSetup()) {
		_selected->run();
	}
	else {
		_displayMenu();
		_animationLayer->run();
		
		_btnNavigateDecrement->run();
		_btnNavigateIncrement->run();
		_btnSelect->run();
		_btnUnselect->run();
	}
}

void Menu::drawTitleScreen(bool isSelected) {
	_matrix->displayScrollingTextBlocking("Marcotendo", 0xF0F0, 40);
}

Program* Menu::getSelectedProgram() const {
	switch (_selectedIndex) {
		case 0: return _quizGame;
		case 1: return _tetris;
		default: return nullptr;
	}
}

void Menu::setQuizGame(QuizGame* quizGame) {
	_quizGame = quizGame;
}

void Menu::setTetris(Tetris* tetris) {
	_tetris = tetris;
}

void Menu::_navigate(int direction) {
	_matrix->clear();
	_selectedIndex = (_selectedIndex + direction + PROGRAM_NB) % PROGRAM_NB;
	_unselect();
	_displayMenu();
}

void Menu::_select() {
	if (_selected) {
		if (!_selected->isSetup()) {
			_selected->setup();
		}
	}
	else {
		Serial.println("Selecting program");
		_selected = getSelectedProgram();
	}
}

void Menu::_unselect() {
	if (_selected) {
		_selected = nullptr;
	}
}

void Menu::_displayMenu() {
    _animationLayer->clear();
	
	uint16_t backgroundColor = (_selectedIndex == 0) ? BLUE : FLUSH_ORANGE;
	uint16_t bandColor = BLACK;
	if (_selected) bandColor = (_selectedIndex == 0) ? CHARTREUSE : CYAN;
	
	Program* programToDisplay = getSelectedProgram();
	if (programToDisplay) programToDisplay->drawTitleScreen(_selected != nullptr);

    _matrix->show();
}
